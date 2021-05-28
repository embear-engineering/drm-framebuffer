/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <xf86drm.h>
#include <xf86drmMode.h>


#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define ALLOC_SIZE 64

static void usage(void)
{
    printf ("\ndrm-framebuffer [OPTIONS...]\n\n"
            "Pipe data to a framebuffer\n\n"
            "  -d dri device /dev/dri/cardN\n"
            "  -l list connectors\n"
            "  -c connector to use (HDMI-A-1, LVDS-1)\n"
            "  -r get resolution dri device and connector needs to be set\n"
            "  -h show this message\n\n");
}

struct type_name {
	unsigned int type;
	const char *name;
};

static const struct type_name connector_type_names[] = {
	{ DRM_MODE_CONNECTOR_Unknown, "unknown" },
	{ DRM_MODE_CONNECTOR_VGA, "VGA" },
	{ DRM_MODE_CONNECTOR_DVII, "DVI-I" },
	{ DRM_MODE_CONNECTOR_DVID, "DVI-D" },
	{ DRM_MODE_CONNECTOR_DVIA, "DVI-A" },
	{ DRM_MODE_CONNECTOR_Composite, "composite" },
	{ DRM_MODE_CONNECTOR_SVIDEO, "s-video" },
	{ DRM_MODE_CONNECTOR_LVDS, "LVDS" },
	{ DRM_MODE_CONNECTOR_Component, "component" },
	{ DRM_MODE_CONNECTOR_9PinDIN, "9-pin DIN" },
	{ DRM_MODE_CONNECTOR_DisplayPort, "DP" },
	{ DRM_MODE_CONNECTOR_HDMIA, "HDMI-A" },
	{ DRM_MODE_CONNECTOR_HDMIB, "HDMI-B" },
	{ DRM_MODE_CONNECTOR_TV, "TV" },
	{ DRM_MODE_CONNECTOR_eDP, "eDP" },
	{ DRM_MODE_CONNECTOR_VIRTUAL, "Virtual" },
	{ DRM_MODE_CONNECTOR_DSI, "DSI" },
	{ DRM_MODE_CONNECTOR_DPI, "DPI" },
};

const char *connector_type_name(unsigned int type) {
    if (type < ARRAY_SIZE(connector_type_names) && type >= 0) {
        return connector_type_names[type].name;
    }

    return "INVALID";
}

int list_connectors(const char *dri_device)
{
    int fd;
    drmModeResPtr res;


    fd = open(dri_device, O_RDWR);
    if (fd < 0) {
        printf("Could not open dri device %s\n", dri_device);
        return -EINVAL;
    }

    res = drmModeGetResources(fd);
    if (!res) {
        printf("Could not get drm resources\n");
        return -EINVAL;
    }


    printf("connectors:");
    for (int i = 0; i < res->count_connectors; i++) {
        drmModeConnectorPtr connector = 0;
        drmModeEncoderPtr encoder = 0;

        printf("\nNumber: %d ", res->connectors[i]);
        connector = drmModeGetConnectorCurrent(fd, res->connectors[i]);
        if (!connector)
            continue;

        printf("Name: %s-%u ", connector_type_name(connector->connector_type), connector->connector_type_id);

        printf("Encoder: %d ", connector->encoder_id);

        encoder = drmModeGetEncoder(fd, connector->encoder_id);
        if (!encoder)
            continue;

        printf("Crtc: %d ", encoder->crtc_id);

        drmModeFreeEncoder(encoder);
        drmModeFreeConnector(connector);
    }
    printf("\n");

    printf("Framebuffers: ");
    for (int i = 0; i < res->count_fbs; i++) {
        printf("%d ", res->fbs[i]);
    }
    printf("\n");

    printf("CRTCs: ");
    for (int i = 0; i < res->count_crtcs; i++) {
        printf("%d ", res->crtcs[i]);
    }
    printf("\n");

    printf("encoders: ");
    for (int i = 0; i < res->count_encoders; i++) {
        printf("%d ", res->encoders[i]);
    }
    printf("\n");

    drmModeFreeResources(res);

    return 0;

}

int get_resolution(const char *dri_device, const char *connector_name)
{
    int err = 0;
    int fd;
    drmModeResPtr res;

    fd = open(dri_device, O_RDWR);
    if (fd < 0) {
        printf("Could not open dri device %s\n", dri_device);
        return -EINVAL;
    }

    res = drmModeGetResources(fd);
    if (!res) {
        printf("Could not get drm resources\n");
        return -EINVAL;
    }

    /* Search the connector provided as argument */
    drmModeConnectorPtr connector = 0;
    for (int i = 0; i < res->count_connectors; i++) {
        char name[32];

        connector = drmModeGetConnectorCurrent(fd, res->connectors[i]);
        if (!connector)
            continue;

        snprintf(name, sizeof(name), "%s-%u", connector_type_name(connector->connector_type),
                connector->connector_type_id);

        if (strncmp(name, connector_name, sizeof(name)) == 0)
                break;

        drmModeFreeConnector(connector);
    }

    if (!connector) {
        printf("Could not find matching connector %s\n", connector_name);
        return -EINVAL;
    }

    /* Get the preferred resolution */
    drmModeModeInfoPtr resolution = 0;
    for (int i = 0; i < connector->count_modes; i++) {
            resolution = &connector->modes[i];
            if (resolution->type & DRM_MODE_TYPE_PREFERRED)
                    break;
    }

    if (!resolution) {
        printf("Could not find preferred resolution\n");
        err = -EINVAL;
        goto error;
    }

    printf("%ux%u\n", resolution->hdisplay, resolution->vdisplay);

error:
    drmModeFreeConnector(connector);
    drmModeFreeResources(res);
    close(fd);
    return err;
}

int get_framebuffer(const char *dri_device, const char *connector_name)
{
    int err;
    int fd;
    drmModeResPtr res;

    fd = open(dri_device, O_RDWR);
    if (fd < 0) {
        printf("Could not open dri device %s\n", dri_device);
        return -EINVAL;
    }

    res = drmModeGetResources(fd);
    if (!res) {
        printf("Could not get drm resources\n");
        return -EINVAL;
    }


    /* Search the connector provided as argument */
    drmModeConnectorPtr connector = 0;
    for (int i = 0; i < res->count_connectors; i++) {
        char name[32];

        connector = drmModeGetConnectorCurrent(fd, res->connectors[i]);
        if (!connector)
            continue;

        snprintf(name, sizeof(name), "%s-%u", connector_type_name(connector->connector_type),
                connector->connector_type_id);

        if (strncmp(name, connector_name, sizeof(name)) == 0)
                break;

        drmModeFreeConnector(connector);
    }

    if (!connector) {
        printf("Could not find matching connector %s\n", connector_name);
        return -EINVAL;
    }

    /* Get the preferred resolution */
    drmModeModeInfoPtr resolution = 0;
    for (int i = 0; i < connector->count_modes; i++) {
            resolution = &connector->modes[i];
            if (resolution->type & DRM_MODE_TYPE_PREFERRED)
                    break;
    }

    if (!resolution) {
        printf("Could not find preferred resolution\n");
        err = -EINVAL;
        goto free_conn;
    }

    struct drm_mode_create_dumb dumb_framebuffer = {
            .height = resolution->vdisplay,
            .width  = resolution->hdisplay,
            .bpp    = 32
    };
    err = ioctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &dumb_framebuffer);
    if (err) {
        printf("Could not create dumb framebuffer\n");
        goto free_conn;
    }

    uint32_t frame_buffer_id;
    err = drmModeAddFB(fd, resolution->hdisplay, resolution->vdisplay,
            24, 32, dumb_framebuffer.pitch, dumb_framebuffer.handle, &frame_buffer_id);
    if (err) {
        printf("Could not add framebuffer to drm\n");
        goto destroy_dumb_framebuffer;
    }

    drmModeEncoderPtr encoder = drmModeGetEncoder(fd, connector->encoder_id);
    if (!encoder) {
        printf("Could not get encoder\n");
        err = -EINVAL;
        goto free_framebuffer;
    }

    drmModeCrtcPtr original_crtc = drmModeGetCrtc(fd, encoder->crtc_id);
    if (!original_crtc) {
        printf("Could not find original crtc\n");
        err = -EINVAL;
        goto free_encoder;
    }

    err = drmModeSetCrtc(fd, encoder->crtc_id, frame_buffer_id, 0, 0, &connector->connector_id, 1, resolution);
    if (err) {
        printf("Could not set new framebuffer for CRTC\n");
        goto free_crtc;
    }

    struct drm_mode_map_dumb mreq;

    memset(&mreq, 0, sizeof(mreq));
    mreq.handle = dumb_framebuffer.handle;

    err = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);
    if (err) {
        printf("Mode map dumb failed\n");
        goto free_crtc;
    }

    uint8_t *framebuffer = mmap(0, dumb_framebuffer.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);
    if (framebuffer == MAP_FAILED) {
        printf("Mode map failed\n");
        goto free_crtc;
    }

    memset(framebuffer, 0, dumb_framebuffer.size);

    size_t total_read = 0;
    while (total_read < dumb_framebuffer.size)
        total_read += read(STDIN_FILENO, &framebuffer[total_read], dumb_framebuffer.size);

    /* Make sure we synchronize the display with the buffer. This also works if page flips are enabled */
    drmModeDirtyFB(fd, frame_buffer_id, NULL, 0);

    while (1) {
        sleep(1);
    }

free_crtc:
    drmModeFreeCrtc(original_crtc);
free_encoder:
    drmModeFreeEncoder(encoder);
free_framebuffer:
    drmModeFreeFB(drmModeGetFB(fd, frame_buffer_id));
destroy_dumb_framebuffer:
    ioctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, dumb_framebuffer);
free_conn:
    drmModeFreeConnector(connector);

    return err;

}

int main(int argc, char** argv)
{
    char *dri_device = 0;
    char *connector = 0;
    int c;
    int list = 0;
    int resolution = 0;

    opterr = 0;
    while ((c = getopt (argc, argv, "d:c:lrh")) != -1) {
        switch (c)
        {
        case 'd':
            dri_device = optarg;
            break;
        case 'c':
            connector = optarg;
            break;
        case 'l':
            list = 1;
            break;
        case 'r':
            resolution = 1;
            break;
        case 'h':
            usage();
            return 1;
        default:
            break;
        }
    }

    if (dri_device == 0) {
        printf("Please set a device\n");
        usage();
        return 3;
    }

    if (list) {
        return list_connectors(dri_device);
    }

    if (connector == 0) {
        printf("Please set a connector\n");
        usage();
        return 4;
    }

    if (resolution) {
        return get_resolution(dri_device, connector);
    }

    get_framebuffer(dri_device, connector);

    return 0;
}


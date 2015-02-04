#include "qeglfshooks.h"
#include <EGL/egl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <unistd.h>
#include <QDebug>

QT_BEGIN_NAMESPACE

class QEglFSSUNXIHooks : public QEglFSHooks
{
public:
    QEglFSSUNXIHooks();
    virtual QSize screenSize() const;
    virtual EGLNativeWindowType createNativeWindow(QPlatformWindow *window, const QSize &size, const QSurfaceFormat &format);
    virtual void destroyNativeWindow(EGLNativeWindowType window);

private:
    QSize mScreenSize;
};

QEglFSSUNXIHooks::QEglFSSUNXIHooks()
{
    int fd = open("/dev/fb0", O_RDONLY);
    if (fd == -1) {
        qFatal("Failed to open fb to detect screen resolution!");
    }

    struct fb_var_screeninfo vinfo;
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        qFatal("Could not get variable screen info");
    }

    close(fd);

    mScreenSize.setWidth(vinfo.xres);
    mScreenSize.setHeight(vinfo.yres);

    qDebug() << "Screen Size = " << mScreenSize;
}

QSize QEglFSSUNXIHooks::screenSize() const
{
    return mScreenSize;
}

EGLNativeWindowType QEglFSSUNXIHooks::createNativeWindow(QPlatformWindow *window, const QSize &size, const QSurfaceFormat &format)
{
    Q_UNUSED(window);
    Q_UNUSED(format);

    mali_native_window *eglWindow = new mali_native_window;
    eglWindow->width = size.width();
    eglWindow->height = size.height();

    return eglWindow;
}

void QEglFSSUNXIHooks::destroyNativeWindow(EGLNativeWindowType window)
{
    delete window;
}

QEglFSSUNXIHooks eglFSSUNXIHooks;
QEglFSHooks *platformHooks = &eglFSSUNXIHooks;

QT_END_NAMESPACE

#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_


#include <algorithm>
#include <cstdint>
#include <mutex>
#include <queue>
#include <thread>
#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <xcb/xproto.h>
#include "camera.hpp"


class Window {
private:
    bool closed_;
    int width_;
    int height_;

    xcb_connection_t* connection_;
    xcb_window_t window_;
    xcb_gcontext_t gcontext_;
    xcb_pixmap_t canvas_;
    xcb_image_t* image_;

    int mouse_x_;
    int mouse_y_;
    bool mouse_pressed_;

    std::queue<xcb_generic_event_t*> waited_events_;

    std::mutex queue_lock_;

public:
    Window(int width = 800, int height = 600);
    ~Window();

public:
    void display() noexcept;

    void handleEvent(Camera& camera) noexcept;

    void draw(std::uint8_t* data, int x, int y, int width, int height) const noexcept;

    bool isClosed() const noexcept {
        return closed_;
    }

private:
    std::queue<xcb_generic_event_t*> pullEvents() noexcept {
        std::queue<xcb_generic_event_t*> ret;
        std::lock_guard<std::mutex> lock(queue_lock_);
        waited_events_.swap(ret);
        return ret;
    }
};

#endif

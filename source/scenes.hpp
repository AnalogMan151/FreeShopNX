#pragma once

#include <switch/types.h>
#include "ui.hpp"

class scene_t
{
public:
    virtual void draw(frame_t& frame) = 0;
    virtual void handle_input(u64 kDown, u64 kHeld) = 0;
};

class title_scene_t
    : public scene_t
{
private:
    u64 m_lastInput = 0;
public:
    void draw(frame_t& frame) override;
    void handle_input(u64 kDown, u64 kHeld) override;
};

class info_scene_t
    : public scene_t
{
public:
    void draw(frame_t& frame) override;
    void handle_input(u64 kDown, u64 kHeld) override;
};

class update_scene_t
    : public scene_t
{
public:
    void draw(frame_t& frame) override;
    void handle_input(u64 kDown, u64 kHeld) override;
};

class about_scene_t
    : public scene_t
{
public:
    void draw(frame_t& frame) override;
    void handle_input(u64 kDown, u64 kHeld) override;
};

class install_scene_t
    : public scene_t
{
public:
    void draw(frame_t& frame) override;
    void handle_input(u64 kDown, u64 kHeld) override;
};

class ticket_scene_t
    : public scene_t
{
  public:
    void draw(frame_t &frame) override;
    void handle_input(u64 kDown, u64 kHeld) override;
};

#pragma once
#include "main.h"
#include <stdexcept>
#include <string>
#include <utility>

/**
 * Wrapper for LVGL Objects
 */
class Object {
public:
  /**
   * Wrap an empty LVGL object pointer
   */
  Object() = default;

  /**
   * Wrap an LVGL object pointer
   *
   * @param iobject The object pointer
   */
  Object(lv_obj_t* iobject);

  /**
   * Destroy the Object wrapper but the lvgl object is left untouched
   * Use remove() to delete the lvgl object
   */
  virtual ~Object() = default;

  /**
   * Create an object. Defaults parent to lv_scr_act() and copy to NULL
   */
  static Object create();

  /**
   * Create an object with the parent parameter. Defaults copy to NULL
   *
   * @param iparent the parent object for this object to be a child of
   */
  static Object create(const Object& iparent);

  /** 
   * Create an object with both the parent and copy parameters
   *
   * @param iparent the parent object for this object to be a child of
   * @param icopy   the object for this object to be a copy of
   */
  static Object create(const Object& iparent, const Object& icopy);

  /**
   * Get the active screen object
   *
   * @return the active screen object (loaded by 'lv_scr_load()')
   */
  static Object ActiveScreen();

  /**
   * Get the top layer object. (Same on every screen and it is above the normal screen layer)
   *
   * @return the top layer object (transparent screen sized lv_obj)
   */
  static Object TopScreen();

  /**
   * Get the system layer object. (Same on every screen and it is above the all other layers) It
   * is used for example by the cursor
   *
   * @return the system layer object (transparent screen sized lv_obj)
   */
  static Object SystemScreen();

  /**
   * Delete this Object and all of its children
   */
  void remove();

  /**
   * Delete all of this Object's children
   */
  Object& clean();

  /** 
   * Mark the object as invalid (see LVGL docs)
   */
  Object& invalidate();

  /** 
   * Set the position of the object
   *
   * @param x the new x position of the object
   * @param y the new y position of the object
   */
  Object& setPosition(lv_coord_t x, lv_coord_t y);

  /** 
   * Set the x position of the object
   *
   * @param x the new x coordinate (measured from left side of the parent)
   */
  Object& setX(lv_coord_t x);

  /** 
   * Set the y position of the object
   *
   * @param y the new y coordinate (measured from top of the parent)
   */
  Object& setY(lv_coord_t y);

  /** 
   * Set the size of the object
   *
   * @param width  the new width of the object
   * @param height the new height of the object
   */
  Object& setSize(lv_coord_t width, lv_coord_t height);

  /** 
   * Set the width of the object
   *
   * @param width the new width of the object
   */
  Object& setWidth(lv_coord_t width);

  /** 
   * Set the height of the object
   *
   * @param height the new height of the object
   */
  Object& setHeight(lv_coord_t height);

  /**
   * Align the object to another object
   *
   * @param base      the object to align to
   * @param alignment the type of alignment
   * @param xShift    the x-coordinate shift after alignment
   * @param yShift    the y-coordinate shift after alignment
   */
  Object& align(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift);

  /**
   * Set a new style for an object
   *
   * @param style the new style
   */
  Object& setStyle(lv_style_t* style);

  /**
   * Return with the screen of an object
   *
   * @return the screen
   */
  Object getScreen() const;

  /**
   * Return the parent of an object
   *
   * @return the parent of object
   */
  Object getParent() const;

  /**
   * Get the coordinates of an object
   *
   * @return The position.
   */
  lv_area_t getPosition() const;

  /**
   * Get the x coordinate of object
   *
   * @return distance of object from the left side of its parent
   */
  lv_coord_t getX() const;

  /**
   * Get the y coordinate of object
   *
   * @return distance of object from the top of its parent
   */
  lv_coord_t getY() const;

  /**
   * Get the width of an object
   *
   * @return the width
   */
  lv_coord_t getWidth() const;

  /**
   * Get the height of an object
   *
   * @return the height
   */
  lv_coord_t getHeight() const;

  /**
   * Get the style pointer of the object (if NULL get style of the parent)
   *
   * @return pointer to a style
   */
  lv_style_t* getStyle() const;

  /**
   * Returns the internal lvgl object pointer. An Object can be converted to an lvgl pointer.
   */
  lv_obj_t* get() const;
  operator lv_obj_t*() const;

  /**
   * Implicitly converts to any object that is derived from Object
   */
  template <typename T, typename = std::enable_if_t<std::is_base_of<Object, T>::value>> operator T&() {
    try {
      return dynamic_cast<T&>(*this);
    } catch (const std::bad_cast& e) {
      throw std::runtime_error(
        "error: failed to implicitly convert ‘Object’ to ‘" + std::string(T::getName()) + "’");
    }
  }

  /**
   * Return the name of the object, used for logging
   */
  static constexpr const char* getName() {
    return "Object";
  }

protected:
  lv_obj_t* object = NULL;
};

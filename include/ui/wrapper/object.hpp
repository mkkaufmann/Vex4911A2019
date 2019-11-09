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
   * Wraps an LVGL object pointer
   *
   * @param iobject The object pointer
   */
  explicit Object(lv_obj_t* iobject);

  /**
   * Destroys the Object wrapper but the lvgl object is left untouched.
   * Use remove() to delete the lvgl object
   */
  virtual ~Object() = default;

  /**
   * Copying the wrapper makes no sense, to copy the object use copy()
   */
  Object(const Object& iobject) = delete;
  Object& operator=(const Object& iobject) = delete;

  /**
   * Moving the wrapper is supported
   */
  Object(Object&& iobject) = default;
  Object& operator=(Object&& iobject) = default;

  /**
   * Returns the name of the object, used for logging
   */
  static constexpr const char* getName() {
    return "Object";
  }

  /**
   * Implicitly converts to any object that is derived from Object
   */
  template <typename T> operator T &&() && {
    static_assert(
      std::is_base_of<Object, T>::value, "cannot implicitly convert ‘Object’ to non-derived type");
    try {
      return std::move(dynamic_cast<T&>(*this));
    } catch (const std::bad_cast& e) {
      throw std::runtime_error(
        "error: failed to implicitly convert ‘Object’ to ‘" + std::string(T::getName()) + "’");
    }
  }

  /**
   * Returns the internal lvgl object pointer.
   */
  lv_obj_t* get() const;

  /*--------------------
   * Create and delete
   *-------------------*/

  /**
   * Create object with no parameters. Defaults parent to lv_scr_act() and copy to NULL
   */
  static Object create();

  /** 
   * Create object with parent parameter and defaults copy to NULL
   *
   * @param iparent the parent object for this object to be a child of
   */
  static Object create(const Object& iparent);

  /** 
   * Create object with both parent and copy parameters
   *
   * @param iparent the parent object for this object to be a child of
   * @param icopy   the object for this object to copy
   */
  static Object create(const Object& iparent, const Object& icopy);

  /** 
   * Delete this Object and all of its children
   */
  void remove();

  /** 
   * Delete all of this Object's children
   */
  Object& clean() &;
  Object&& clean() &&;

  /** 
   * Mark the object as invalid (see LVGL docs)
   */
  Object& invalidate() &;
  Object&& invalidate() &&;

  /*=====================
   * Setter functions
   *====================*/

  /*--------------------
   * Parent/children set
   *--------------------*/

  /** 
   * Set the parent of this Object
   *
   * @param iparent the parent for this object to become a child of
   */
  Object& setParent(const Object& iparent) &;
  Object&& setParent(const Object& iparent) &&;

  /*--------------------
   * Coordinate set
   * ------------------*/

  /** 
   * Set the position of the object
   *
   * @param x the new x position of the object
   * @param y the new y position of the object
   */
  Object& setPosition(lv_coord_t x, lv_coord_t y) &;
  Object&& setPosition(lv_coord_t x, lv_coord_t y) &&;

  /** 
   * Set the x position of the object
   *
   * @param x the new x coordinate (measured from left side of the parent)
   */
  Object& setX(lv_coord_t x) &;
  Object&& setX(lv_coord_t x) &&;

  /** 
   * Set the y position of the object
   *
   * @param y the new y coordinate (measured from top of the parent)
   */
  Object& setY(lv_coord_t y) &;
  Object&& setY(lv_coord_t y) &&;

  /** 
   * Set the size of the object
   *
   * @param width  the new width of the object
   * @param height the new height of the object
   */
  Object& setSize(lv_coord_t width, lv_coord_t height) &;
  Object&& setSize(lv_coord_t width, lv_coord_t height) &&;

  /** 
   * Set the width of the object
   *
   * @param width the new width of the object
   */
  Object& setWidth(lv_coord_t width) &;
  Object&& setWidth(lv_coord_t width) &&;

  /** 
   * Set the height of the object
   *
   * @param height the new height of the object
   */
  Object& setHeight(lv_coord_t height) &;
  Object&& setHeight(lv_coord_t height) &&;

  /**
   * Align the object to another object
   *
   * @param base      the object to align to
   * @param alignment the type of alignment
   * @param xShift    the x-coordinate shift after alignment
   * @param yShift    the y-coordinate shift after alignment
   */
  Object& align(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) &;
  Object&& align(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) &&;

  /** 
   * Align the object to its parent
   *
   * @param alignment the type of alignment
   * @param xShift    the x-coordinate shift after alignment
   * @param yShift    the y-coordinate shift after alignment
   */
  Object& alignToParent(lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) &;
  Object&& alignToParent(lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) &&;

  /** 
   * Align the object to another object
   *
   * @param base      the object to align to
   * @param alignment the type of alignment
   * @param xShift    the x-coordinate shift after alignment
   * @param yShift    the y-coordinate shift after alignment
   */
  Object& alignOrigo(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) &;
  Object&& alignOrigo(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) &&;

  /** 
   * Realign the object based on the last alignment
   */
  Object& realign() &;
  Object&& realign() &&;

  /** 
   * Enable automatic realignment on resize
   *
   * @param enabled whether or not realignment is enabled
   */
  Object& setAutoRealign(bool enabled) &;
  Object&& setAutoRealign(bool enabled) &&;

  /*---------------------
   * Appearance set
   *--------------------*/

  /**
   * Set a new style for an object
   *
   * @param style the new style
   */
  Object& setStyle(lv_style_t* style) &;
  Object&& setStyle(lv_style_t* style) &&;

  /**
   * Notify an object about its style is modified
   */
  void refreshStyle();

  /*-----------------
   * Attribute set
   *----------------*/

  /**
   * Hide an object. It won't be visible and clickable.
   *
   * @param enabled true: hide the object
   */
  void setHidden(bool enabled);

  /**
   * Enable or disable the clicking of an object
   *
   * @param enabled true: make the object clickable
   */
  void setClick(bool enabled);

  /**
   * Enable to bring this object to the foreground if it or any of its children is clicked
   *
   * @param enabled true: enable the auto top feature
   */
  void setTop(bool enabled);

  /**
   * Enable the dragging of an object
   *
   * @param enabled true: make the object dragable
   */
  void setDrag(bool enabled);

  /**
   * Enable the throwing of an object after is is dragged
   *
   * @param enabled true: enable the drag throw
   */
  void setDragThrow(bool enabled);

  /**
   * Enable to use parent for drag related operations. If trying to drag the object the parent will be moved instead
   *
   * @param enabled true: enable the 'drag parent' for the object
   */
  void setDragParent(bool enabled);

  /**
   * Set the opa scale enable parameter (required to set opa_scale with `lv_obj_set_opa_scale()`)
   *
   * @param enabled true: opa scaling is enabled for this object and all children; false: no opa scaling
   */
  void setOpacityScaleEnable(bool enabled);

  /**
   * Set the opa scale of an object
   *
   * @param opa_scale a factor to scale down opacity [0..255]
   */
  void setOpacityScale(lv_opa_t opa_scale);

  /**
   * Set a bit or bits in the protect filed
   *
   * @param prot 'OR'-ed values from `lv_protect_t`
   */
  void setProtect(uint8_t prot);

  /**
   * Clear a bit or bits in the protect filed
   *
   * @param prot 'OR'-ed values from `lv_protect_t`
   */
  void clearProtect(uint8_t prot);

  /*----------------
   * Other set
   *--------------*/

#ifdef LV_OBJ_FREE_NUM_TYPE
  /**
   * Set an application specific number for an object. It can help to identify objects in the application.
   *
   * @param free_num the new free number
   */
  void setFreeNum(LV_OBJ_FREE_NUM_TYPE free_num);
#endif

#if LV_OBJ_FREE_PTR != 0
  /**
   * Set an application specific  pointer for an object. It can help to identify objects in the application.
   *
   * @param free_p the new free pinter
   */
  void setFreePtr(void* free_p);
#endif

#if USE_LV_ANIMATION
  /**
   * Animate an object
   *
   * @param type  type of animation from 'lv_anim_builtin_t'. 'OR' it with ANIM_IN or ANIM_OUT
   * @param time  time of animation in milliseconds
   * @param delay delay before the animation in milliseconds
   * @param cb    a function to call when the animation is ready
   */
  void animate(lv_anim_builtin_t type, uint16_t time, uint16_t delay, void (*cb)(lv_obj_t*));
#endif

  /*=======================
   * Getter functions
   *======================*/

  /*------------------
   * Screen get
   *-----------------*/

  /**
   * Return with the active screen
   *
   * @return the active screen object (loaded by 'lv_scr_load()')
   */
  static Object ActiveScreen();

  /**
   * Return with the top layer. (Same on every screen and it is above the normal screen layer)
   *
   * @return the top layer object (transparent screen sized lv_obj)
   */
  static Object TopScreen();

  /**
   * Return with the system layer. (Same on every screen and it is above the all other layers) It is used for example by
   * the cursor
   *
   * @return the system layer object (transparent screen sized lv_obj)
   */
  static Object SystemScreen();

  /**
   * Return with the screen of an object
   *
   * @return the screen
   */
  Object getScreen() const;

  /*---------------------
   * Parent/children get
   *--------------------*/

  /**
   * Returns with the parent of an object
   *
   * @return the parent of object
   */
  Object getParent() const;

  /**
   * Iterate through the children of an object (start from the "youngest, lastly created")
   *
   * @param child NULL at first call to get the next children and the previous return value later
   *
   * @return the child after 'act_child' or NULL if no more child
   */
  Object getChild(const Object& child) const;

  /**
   * Iterate through the children of an object (start from the "oldest", firstly created)
   *
   * @param child NULL at first call to get the next children and the previous return value later
   *
   * @return the child after 'act_child' or NULL if no more child
   */
  Object getChildBack(const Object& child) const;

  /**
   * Count the children of an object (only children directly on object)
   *
   * @return children number of object
   */
  uint16_t countChildren() const;

  /*---------------------
   * Coordinate get
   *--------------------*/

  /**
   * Copy the coordinates of an object to an area
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
   * Get the extended size attribute of an object
   *
   * @return the extended size attribute
   */
  lv_coord_t getExtSize() const;

  /**
   * Get the automatic realign property of the object.
   *
   * @return true: auto realign is enabled; false: auto realign is disabled
   */
  bool getAutoRealign();

  /*-----------------
   * Appearance get
   *---------------*/

  /**
   * Get the style pointer of an object (if NULL get style of the parent)
   *
   * @return pointer to a style
   */
  lv_style_t* getStyle() const;

  /*-----------------
   * Attribute get
   *----------------*/

  /**
   * Get the hidden attribute of an object
   *
   * @return true: the object is hidden
   */
  bool getHidden() const;

  /**
   * Get the click enable attribute of an object
   *
   * @return true: the object is clickable
   */
  bool getClick() const;

  /**
   * Get the top enable attribute of an object
   *
   * @return true: the auto top feature is enabled
   */
  bool getTop() const;

  /**
   * Get the drag enable attribute of an object
   *
   * @return true: the object is dragable
   */
  bool getDrag() const;

  /**
   * Get the drag throw enable attribute of an object
   *
   * @return true: drag throw is enabled
   */
  bool getDragThrow() const;

  /**
   * Get the drag parent attribute of an object
   *
   * @return true: drag parent is enabled
   */
  bool getDragParent() const;

  /**
   * Get the opa scale enable parameter
   *
   * @return true: opa scaling is enabled for this object and all children; false: no opa scaling
   */
  lv_opa_t getOpacityScaleEnable() const;

  /**
   * Get the opa scale parameter of an object
   *
   * @return opa scale [0..255]
   */
  lv_opa_t getOpacityScale() const;

  /**
   * Get the protect field of an object
   *
   * @return protect field ('OR'ed values of `lv_protect_t`)
   */
  uint8_t getProtect() const;

  /**
   * Check at least one bit of a given protect bitfield is set
   *
   * @param prot protect bits to test ('OR'ed values of `lv_protect_t`)
   *
   * @return false: none of the given bits are set, true: at least one bit is set
   */
  bool isProtected(uint8_t prot) const;

  /*------------------
   * Other get
   *-----------------*/

  /**
   * Get object's and its ancestors type. Put their name in `type_buf` starting with the current type. E.g.
   * buf.type[0]="lv_btn", buf.type[1]="lv_cont", buf.type[2]="lv_obj"
   *
   * @return the type.
   */
  lv_obj_type_t getType() const;

#ifdef LV_OBJ_FREE_NUM_TYPE
  /**
   * Get the free number
   *
   * @return the free number
   */
  LV_OBJ_FREE_NUM_TYPE getFreeNum() const;
#endif

#if LV_OBJ_FREE_PTR != 0
  /**
   * Get the free pointer
   *
   * @return the free pointer
   */
  void* getFreePtr() const;
#endif

#if USE_LV_GROUP
  /**
   * Get the group of the object
   *
   * @return the pointer to group of the object
   */
  void* getGroup() const;

  /**
   * Tell whether the object is the focused object of a group or not.
   *
   * @return true: the object is focused, false: the object is not focused or not in a group
   */
  bool isFocused() const;

#endif

protected:
  lv_obj_t* object = NULL;
};

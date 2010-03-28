/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiAttributeType.h"
#include "nuiTypeTraits.h"
#include "nuiBorder.h"
#include "nuiObject.h"

class nuiPoint;
class nuiRange;

class nuiVariant
{
public:
  nuiVariant()
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mType = nuiAttributeTypeTrait<void>::mTypeId;
  }
  
  // POD CTOR
  template <typename Type>
  nuiVariant(Type data)
  {
    mIsPointer = is_pointer<Type>::value;
    mIsObject = is_base_of<nuiObject, Type>::value;
    mType = nuiAttributeTypeTrait<Type>::mTypeId;
    mIsPOD = false;
    NGL_ASSERT(0);
  }
 
#define CTOR(TYPE)\
  nuiVariant(TYPE data)\
  {\
    mIsPointer = is_pointer<TYPE>::value;\
    mIsObject = is_base_of<nuiObject, TYPE>::value;\
    mType = nuiAttributeTypeTrait<TYPE>::mTypeId;\
    mIsPOD = true;\
    if (is_same_type<TYPE, int8>::value || is_same_type<TYPE, int16>::value || is_same_type<TYPE, int32>::value || is_same_type<TYPE, int64>::value)\
    {\
      mData.mInt = data;\
    }\
    else if (is_same_type<TYPE, uint8>::value || is_same_type<TYPE, uint16>::value || is_same_type<TYPE, uint32>::value || is_same_type<TYPE, uint64>::value)\
    {\
      mData.mUInt = data;\
    }\
    else if (is_same_type<TYPE, float>::value || is_same_type<TYPE, double>::value)\
    {\
      mData.mFloat = data;\
    }\
  }

  CTOR(int8);
  CTOR(int32);
  CTOR(int64);
  
  CTOR(uint8);
  CTOR(uint32);
  CTOR(uint64);
  
  CTOR(float);
  CTOR(double);
#undef CTOR
  
  
  nuiVariant(bool set)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<bool>::mTypeId;
    mIsPOD = true;

    mData.mBool = set;
  }
  
  // Pointer CTOR
  template <typename Type>
  nuiVariant(Type* pData)
  {
    mIsPointer = true;
    mIsObject = is_base_of<nuiObject, Type>::value;
    mType = nuiAttributeTypeTrait<Type*>::mTypeId;
    mData.mpPointer = (void*)pData;
    mIsPOD = false;

    if (mIsObject)
      mData.mpObject->Acquire();
  }
  
  // Classes CTORs
  nuiVariant(const nglString& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mType = nuiAttributeTypeTrait<nglString>::mTypeId;
    mString = rData;
  }
  
  nuiVariant(const nglPath& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mType = nuiAttributeTypeTrait<nglPath>::mTypeId;
    mString = rData.GetPathName();
  }
  
  nuiVariant(const nuiColor& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mType = nuiAttributeTypeTrait<nuiColor>::mTypeId;
    mColor = rData;
  }
  
  nuiVariant(const nuiRect& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mType = nuiAttributeTypeTrait<nuiRect>::mTypeId;
    mRect = rData;
  }
  
  nuiVariant(const nuiMatrix& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mType = nuiAttributeTypeTrait<nuiRect>::mTypeId;
    NGL_ASSERT(0);
  }
  
  nuiVariant(const nuiBorder& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiBorder>::mTypeId;
    mIsPOD = false;
    NGL_ASSERT(0);
  }
  
  nuiVariant(const nuiPoint& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiBorder>::mTypeId;
    mIsPOD = false;
    NGL_ASSERT(0);
  }
  
  nuiVariant(const nuiRange& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiRange>::mTypeId;
    mIsPOD = false;
    NGL_ASSERT(0);
  }
  
  nuiVariant(const nuiVector& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiVector>::mTypeId;
    mIsPOD = false;
    NGL_ASSERT(0);
  }
  

  
  // Copy CTOR
  nuiVariant(const nuiVariant& rObject)
  {
    mData = rObject.mData;
    
    mString = rObject.mString;
    mColor = rObject.mColor;
    mRect = rObject.mRect;
    
    mType = rObject.mType;
    
    mIsPointer = rObject.mIsPointer;
    mIsObject = rObject.mIsObject;
    mIsPOD = rObject.mIsPOD;

    if (mIsObject)
      mData.mpObject->Acquire();
  }
  
  // DTOR:
  ~nuiVariant()
  {
    if (mIsObject)
      mData.mpObject->Release();
  }
  
  nuiVariant& operator=(const nuiVariant& rObject)
  {
    if (mIsObject)
      mData.mpObject->Release();
    
    mData = rObject.mData;
    
    mString = rObject.mString;
    mColor = rObject.mColor;
    mRect = rObject.mRect;
    
    mType = rObject.mType;
    
    mIsPointer = rObject.mIsPointer;
    mIsObject = rObject.mIsObject;
    mIsPOD = rObject.mIsPOD;
    
    if (mIsObject)
      mData.mpObject->Acquire();
    return *this;
  }
  
  nuiAttributeType GetType() const
  {
    return mType;
  }
  
  void Clear()
  {
    if (mIsObject)
      mData.mpObject->Release();

    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mType = nuiAttributeTypeTrait<void>::mTypeId;
  }
  
  bool IsVoid() const
  {
    return mType == nuiAttributeTypeTrait<void>::mTypeId;
  }
  
  bool IsPointer() const
  {
    return mIsPointer;
  }
  
  bool IsObject() const
  {
    return mIsObject;
  }
  
  bool IsPOD() const
  {
    return mIsPOD;
  }
  
  // nglString Cast:
  operator nglString() const
  {
    if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
      return mString;
    
    if (mIsPointer)
      return nglString::Null;
    
    if (mType == nuiAttributeTypeTrait<nuiRect>::mTypeId)
      return mRect.GetValue();
    
    if (mType == nuiAttributeTypeTrait<nuiColor>::mTypeId)
      return mColor.GetValue();
    
    if (mType == nuiAttributeTypeTrait<nglPath>::mTypeId)
      return mString;
    
    nglString str;
    if (mType == nuiAttributeTypeTrait<float>::mTypeId
        || mType == nuiAttributeTypeTrait<double>::mTypeId)
      str.Add(mData.mFloat);
    else if (mType == nuiAttributeTypeTrait<int8>::mTypeId 
             || mType == nuiAttributeTypeTrait<int16>::mTypeId
             || mType == nuiAttributeTypeTrait<int32>::mTypeId
             || mType == nuiAttributeTypeTrait<int64>::mTypeId)
      str.Add(mData.mInt);
    else if (mType == nuiAttributeTypeTrait<uint8>::mTypeId
             || mType == nuiAttributeTypeTrait<uint16>::mTypeId
             || mType == nuiAttributeTypeTrait<uint32>::mTypeId
             || mType == nuiAttributeTypeTrait<uint64>::mTypeId)
      str.Add(mData.mUInt);
    
    return str;
  }
  
  // nglPath Cast:
  operator nglPath() const
  {
    if (mType == nuiAttributeTypeTrait<nglPath>::mTypeId || mType == nuiAttributeTypeTrait<nglString>::mTypeId)
      return mString;

    return nglPath();
  }
  
  // POD Cast:
  template<typename Type>
  operator Type() const
  {
    if (nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<float>::mTypeId
        || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<double>::mTypeId)
    {
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType == nuiAttributeTypeTrait<double>::mTypeId)
        return (Type)mData.mFloat;
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
        return (Type)mString.GetCDouble();
    }
    else if (nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<int8>::mTypeId 
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<int16>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<int32>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<int64>::mTypeId)
    {
      if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType == nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId
          || mType == nuiAttributeTypeTrait<int8>::mTypeId || mType == nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId)
        return (Type)mData.mInt;
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType == nuiAttributeTypeTrait<double>::mTypeId)
        return (Type)ToBelow(mData.mFloat);
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
        return (Type)mString.GetCInt64();
    }
    else if (nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<uint8>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<uint16>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<uint32>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<uint64>::mTypeId)
    {
      if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType == nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId
          || mType == nuiAttributeTypeTrait<int8>::mTypeId || mType == nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId)
        return (Type)mData.mUInt;
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType == nuiAttributeTypeTrait<double>::mTypeId)
        return (Type)ToBelow(mData.mFloat);
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
        return (Type)mString.GetCUInt64();
    }
    
    NGL_ASSERT(0);
  }

  operator bool() const
  {
    if (mType == nuiAttributeTypeTrait<bool>::mTypeId)
      return mData.mBool;

    if (mType == nuiAttributeTypeTrait<float>::mTypeId
        || mType == nuiAttributeTypeTrait<double>::mTypeId)
    {
      return (bool)mData.mFloat;
    }
    else if (mType == nuiAttributeTypeTrait<int8>::mTypeId 
             || mType == nuiAttributeTypeTrait<int16>::mTypeId
             || mType == nuiAttributeTypeTrait<int32>::mTypeId
             || mType == nuiAttributeTypeTrait<int64>::mTypeId)
    {
      return (bool)mData.mInt;
    }
    else if (mType == nuiAttributeTypeTrait<uint8>::mTypeId
             || mType == nuiAttributeTypeTrait<uint16>::mTypeId
             || mType == nuiAttributeTypeTrait<uint32>::mTypeId
             || mType == nuiAttributeTypeTrait<uint64>::mTypeId)
    {
      return (bool)mData.mUInt;
    }
    
    return false;
  }

  operator nuiRect() const
  {
    if (mType == nuiAttributeTypeTrait<nuiRect>::mTypeId)
      return mRect;
    if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
    {
      nuiRect r;
      r.SetValue(mString);
      return r;
    }
    return nuiRect();
  }
  
  operator nuiColor() const
  {
    if (mType == nuiAttributeTypeTrait<nuiColor>::mTypeId)
      return mColor;
    if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
      return nuiColor(mString);
    return nuiColor();
  }
  
  operator nuiMatrix() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiMatrix>::mTypeId)
      return nuiMatrix();
    return nuiMatrix();
  }
  
  operator nuiBorder() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiBorder>::mTypeId)
      return nuiBorder();
    return nuiBorder();
  }
  
  operator nuiPoint() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiPoint>::mTypeId)
      return nuiPoint();
    return nuiPoint();
  }
  
  operator nuiRange() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiRange>::mTypeId)
      return nuiRange();
    return nuiRange();
  }

  operator nuiVector() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiVector>::mTypeId)
      return nuiVector();
    return nuiVector();
  }
  
  template<typename Type>
  operator Type*() const
  {
    if (nuiAttributeTypeTrait<Type*>::mTypeId == mType)
      return reinterpret_cast<Type*>(mData.mpPointer);
    
    if (!mIsPointer)
      return NULL;
    
    if (mIsObject && is_base_of<nuiObject, Type>::value)
      return dynamic_cast<Type*>(reinterpret_cast<nuiObject*>(mData.mpPointer));
    
    return NULL;
  }
  
  template<typename Type>
  operator const Type*() const
  {
    if (nuiAttributeTypeTrait<Type*>::mTypeId == mType)
      return reinterpret_cast<const Type*>(mData.mpPointer);
    
    if (!mIsPointer)
      return NULL;
    
    if (mIsObject && is_base_of<nuiObject, Type>::value)
      return dynamic_cast<const Type*>(reinterpret_cast<nuiObject*>(mData.mpPointer));
    
    return NULL;
  }
  
private:
  nuiAttributeType mType;
  union 
  {
    int64 mInt;
    uint64 mUInt;
    double mFloat;
    bool mBool;
    
    void* mpPointer;
    nuiObject* mpObject;    
  } mData;
  
  nglString mString;
  nuiRect mRect;
  nuiColor mColor;
  
  bool mIsPointer : 1;
  bool mIsObject : 1;
  bool mIsPOD : 1;
};


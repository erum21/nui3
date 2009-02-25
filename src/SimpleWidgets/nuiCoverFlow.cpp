/*
 *  nuiCoverFlow.cpp
 *  nui3
 *
 *  Created by meeloo on 2/25/09.
 *  Copyright 2009 mxp4. All rights reserved.
 *
 */

#include "nuiCoverFlow.h"

nuiCoverFlow::nuiCoverFlow()
: mSelectedImage(0), mBackground(0, 0, 0, 255), mReflectionStart(0.3), mReflectionEnd(0.0), mDrawBackground(true)
{
}

nuiCoverFlow::~nuiCoverFlow()
{
  for (uint32 i = 0; i < mImages.size(); i++)
    mImages[i]->Release();
}

bool nuiCoverFlow::Draw(nuiDrawContext* pContext)
{
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);

  if (mDrawBackground)
  {
    // Draw a rectangle with the background color:
    nuiRect r(GetRect().Size());
    pContext->SetFillColor(mBackground);
    pContext->DrawRect(r, eFillShape);          
  }

  nuiTexture* pTex = mImages[mSelectedImage];
  // Draw the selected image:
  float height = mRect.GetHeight();
  float width = mRect.GetWidth();
  float top = height * .05;
  float bot = height - 100;
  float h = bot - top;
  float w = h;
  float left = (width - w) * .5;
  float right = width - left;
  nuiRect r(left, top, w, h);
  float imgw = pTex->GetWidth();
  float imgh = pTex->GetHeight();
  float s = MAX(imgw, imgh);
  float ratiow = imgw / s;
  float ratioh = imgh / s;
  nuiRect imgrect(0.0f, 0.0f, ratiow * w, ratioh * h);
  imgrect.SetPosition(nuiCenter, r);
  float tw = imgw;
  float th = imgh;
  pTex->ImageToTextureCoord(tw, th);
  
  nuiRenderArray array(GL_TRIANGLE_STRIP);
  array.EnableArray(nuiRenderArray::eColor, true);
  array.EnableArray(nuiRenderArray::eTexCoord, true);
  array.SetColor(nuiColor(255, 255, 255, 255));
  array.SetVertex(imgrect.Left(), imgrect.Top());
  array.SetTexCoords(0, 0);
  array.PushVertex();
  
  array.SetVertex(imgrect.Right(), imgrect.Top());
  array.SetTexCoords(tw, 0);
  array.PushVertex();

  array.SetVertex(imgrect.Left(), imgrect.Bottom());
  array.SetTexCoords(0, th);
  array.PushVertex();

  array.SetVertex(imgrect.Right(), imgrect.Bottom());
  array.SetTexCoords(tw, th);
  array.PushVertex();
  
  ////
  array.SetColor(nuiColor(1.0, 1.0, 1.0, mReflectionStart));
  array.SetVertex(imgrect.Left(), imgrect.Bottom());
  array.SetTexCoords(0, th);
  array.PushVertex();
  
  array.SetVertex(imgrect.Right(), imgrect.Bottom());
  array.SetTexCoords(tw, th);
  array.PushVertex();
  
  array.SetColor(nuiColor(1.0, 1.0, 1.0, mReflectionEnd));
  array.SetVertex(imgrect.Left(), imgrect.Bottom() + imgrect.GetHeight());
  array.SetTexCoords(0, 0);
  array.PushVertex();

  array.SetVertex(imgrect.Right(), imgrect.Bottom() + imgrect.GetHeight());
  array.SetTexCoords(tw, 0);
  array.PushVertex();
  
  pContext->EnableTexturing(true);
  pContext->SetTexture(pTex);
  pContext->DrawArray(array);
  
  return false;
}

bool nuiCoverFlow::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);  
  return true;
}

uint32 nuiCoverFlow::AddImage(nuiTexture* pTexture, nuiTexture* pBefore)
{
  pTexture->Acquire();
  Invalidate();
  if (!pBefore)
  {
    mImages.push_back(pTexture);
    return mImages.size() - 1;
  }

  for (uint32 i = 0; i < mImages.size(); i++)
  {
    if (mImages[i] == pBefore)
    {
      mImages.insert(mImages.begin() + i, pTexture);
      return i;
    }
  }

  // pBefore not found? Add to back
  mImages.push_back(pTexture);
  return mImages.size() - 1;
}

uint32 nuiCoverFlow::AddImage(nuiTexture* pTexture, uint32 index)
{
  pTexture->Acquire();
  Invalidate();
  mImages.insert(mImages.begin() + index, pTexture);
  return index;
}

void nuiCoverFlow::DelImage(nuiTexture* pTexture)
{
  for (uint32 i = 0; i < mImages.size(); i++)
  {
    if (mImages[i] == pTexture)
    {
      mImages[i]->Release();
      mImages.erase(mImages.begin() + i);
      Invalidate();
      SelectImage(mSelectedImage);
      return;
    }
  }
}

void nuiCoverFlow::DelImage(uint32 index)
{
  mImages[index]->Release();
  mImages.erase(mImages.begin() + index);
  SelectImage(mSelectedImage);
  Invalidate();
}

void nuiCoverFlow::SelectImage(nuiTexture* pTexture)
{
  for (uint32 i = 0; i < mImages.size(); i++)
  {
    if (mImages[i] == pTexture)
    {
      SelectImage(i);
      return;
    }
  }
}

void nuiCoverFlow::SelectImage(int32 index)
{
  if (index >= mImages.size())
    index = mImages.size() - 1;
  else if (index < 0)
    index = 0;
  mSelectedImage = index;
  
  Invalidate();
}


const std::vector<nuiTexture*>& nuiCoverFlow::GetImages() const
{
  return mImages;
}

void nuiCoverFlow::SetBackground(const nuiColor& rColor)
{
  mBackground = rColor;
  Invalidate();
}

const nuiColor& nuiCoverFlow::GetBackground() const
{
  return mBackground;
}

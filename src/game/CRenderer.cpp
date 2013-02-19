/*
 * Copyright (C) 2012-2013 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
 *
 * This file is part of Pexeso3D.
 *
 * Pexeso3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pexeso3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pexeso3D.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Implementation of renderer class
 */

#include <QDebug>
#include <QMatrix4x4>

#include "CRenderer.h"
#include "Global.h"
#include "CScene.h"
#include "CTrackBall.h"



/* initialize class constants */
const QColor CRenderer::DEF_SELECTION_COLOR = QColor(0x4A, 0xB9, 0xED);


/* private global variables */
namespace {

const double g_near_plane = 0.01f;
const double g_far_plane = 20000.0f;
const double g_near_far_dist = g_far_plane - g_near_plane;
const double g_persp_angle = 45.0f;    // angle in perspective projection

}  // End of private namespace



/**
 */
bool CRenderer::init(int w, int h)
{
  m_w = w;
  m_h = h;

  /* reset OpenGL errors */
  glGetError();

  /* set up viewport area */
  glViewport(0, 0, m_w, m_h);

  /* set up projection */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(g_persp_angle, (((float) m_w) / ((float) m_h)), g_near_plane, g_far_plane);

  /* reset back to model-view matrix */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* do other settings */
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClearStencil(0);
  glEnable(GL_MULTISAMPLE);  // antialiasing (enable it in case it was disabled by someone else)
  glEnable(GL_DEPTH_TEST);   // enable depth testing
  glEnable(GL_CULL_FACE);    // enable face culling

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glDepthFunc(GL_LEQUAL);

  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  /* prepare for rendering */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glLoadIdentity();

  /* reset the color, so textures won't be
     mixed up with previous color settings */
  glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

  /* check on OpenGL errors */
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    qDebug() << "Failed to initialize renderer: " << OpenGL::glErrorToString(error);
    return false;
  }

  return true;
}


/**
 */
bool CRenderer::deinit(void)
{
  /* reset OpenGL errors */
  glGetError();

  /*reset the projection back to orthographic */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, m_w, m_h, 0.0f, 0.0f, 1.0f);

  /* reset back to model-view matrix */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* do other saettings */
  glDisable(GL_DEPTH_TEST);   // depth testing is initially disabled
  glDisable(GL_CULL_FACE);    // face culling is initially disabled

  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glStencilFunc(GL_ALWAYS, 0, 0xFF);
  glBlendFunc(GL_ONE, GL_ZERO);
  glDepthFunc(GL_LESS);
  glLineWidth(1.0f);           // the intial value is 1

  /* check on OpenGL errors */
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    qDebug() << "Failed to deinitialize renderer: " << OpenGL::glErrorToString(error);
    return false;
  }

  return true;
}


/**
 */
void CRenderer::resize(int w, int h)
{
  m_w = w;
  m_h = h;

  glViewport(0, 0, m_w, m_h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(g_persp_angle, (((float) m_w) / ((float) m_h)), g_near_plane, g_far_plane);

  glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();

  return;
}


/**
 */
void CRenderer::renderBackground(const CScene & scene)
{
  /* switch projection for background rendering */
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  /* query scene for necessary data */
  const CTextureStore & tex_store = scene.getTextures();
  int background_tex_ind = scene.getBackgroundTextureIndex();

  /* this command disables writing to depth buffer,
     this is important otherwise glVertex2f implicitly uses
     0.0f as depth, which is too close to camera and thus pixels
     of the scene do not pass the depth test */
  glDepthMask(GL_FALSE);

  /* render background */
  tex_store.bindTexture(background_tex_ind);

  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
  glEnd();

  tex_store.unbindTexture();

  /* and reenable depth testing after */
  glDepthMask(GL_TRUE);

  /* and switch the projection back */
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  return;
}


/**
 */
void CRenderer::renderScene(const CScene & scene)
{
  /* check if rendering is enabled */
  if (m_mode == RM_NULL)
  {
    return;
  }

  /* query the scene information and set up variables */
  CScene::CConstModelIter model_it = scene.getModelIterBegin();
  CScene::CConstModelIter model_it_end = scene.getModelIterEnd();
  const CTextureStore & tex_store = scene.getTextures();  // a container with textures
  int cover_tex_ind = scene.getCoverTextureIndex();       // index of cover texture
  GLfloat inc_c = 1.0f / scene.getModelCount();           // how much different will next model's color be
  GLfloat col_r = 1.0f;                                   // normal red color component
  GLfloat col_g = 1.0f;                                   // normal green color component
  GLfloat col_b = 1.0f;                                   // normal blue color component
  GLfloat col_a = 0.0f;                                   // normal alpha color component
  unsigned int col = 0;                                   // a color used in color mode

  /* set the color to be used for wireframe rendering */
  if (m_mode == RM_WIRED)
  {
    const QColor & col = scene.getWFColor();  // color used for wireframe drawing
    col_r = col.redF();
    col_g = col.greenF();
    col_b = col.blueF();
    col_a = col.alphaF();
  }

  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // when the stencil test passes write to stencil buffer
  glColor4f(col_r, col_g, col_b, col_a);      // set the rendering color
  glLineWidth(scene.getWFLineWidth());        // set the wireframe line width

  /* this command will cause that an offset will be
     added to each rendered polygon's depth value.
     Without this setting outlines of polygons would
     mess up z-value (depth) and would not look nice,
     i.e they could become dashed or have other kinds
     of artifacts on them */
  glPolygonOffset(1.0f, 1.0f);

  /* if the trackball is set, rotate the scene according to trackball */
  if (m_track_ball)
  {
    QMatrix4x4 m;
    m.rotate(m_track_ball->getRotation());
    glMultMatrixd(m.constData());
  }

  /* move the scene to the center of the view */
  {
    Maths::SVector3D scene_center = scene.getBBox().getCenter();
    glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
  }

  /*
   * enable stenciling - this is used for mouse selection tests
   * and it has to be done here and not in initialisation,
   * because drawing background will make it malfunction
   */
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);

  /* render models */
  while (model_it != model_it_end)
  {
    /* do not render the model if it is disabled */
    if ((*model_it)->isDeleted())
    {
      ++model_it;
      continue;
    }

    /* query model data */
    const CVertexBuffer & vb = (*model_it)->getVertexBuffer();
    const CIndexBuffer & ib = (*model_it)->getIndexBuffer();

    glStencilFunc(GL_ALWAYS, (*model_it)->getID(), 0xFF);

    /* render a single model */
    vb.bind();
    ib.bind();

    glEnable(GL_POLYGON_OFFSET_FILL);   // this will enable adding offset

    if (m_mode == RM_NORMAL)
    { // in normal mode select the texture according to model state
      if ((*model_it)->isCovered())
      {
        tex_store.bindTexture(cover_tex_ind);
      }
      else
      {
        int tex_ind = (*model_it)->getTextureIndex();
        tex_store.bindTexture(tex_ind);
      }

      glDrawElements(GL_QUADS, ib.getLength(), ib.getType(), NULL);

      tex_store.unbindTexture();
    }
    else if (m_mode == RM_UNCOVERED)
    { // draw the models with "uncovered texture"
      int tex_ind = (*model_it)->getTextureIndex();
      qDebug() << "tex_ind == " << tex_ind;
      tex_store.bindTexture(tex_ind);

      glDrawElements(GL_QUADS, ib.getLength(), ib.getType(), NULL);

      tex_store.unbindTexture();
    }
    else if (m_mode == RM_COLORED)
    { // draw models with a given color
      col++;
      glColor3f(col & 1, col & 2, col & 4);
      glDrawElements(GL_QUADS, ib.getLength(), ib.getType(), NULL);
    }
    else if (m_mode == RM_GREYED)
    {
      col_r -= inc_c;
      glColor3f(col_r, col_r, col_r);
      glDrawElements(GL_QUADS, ib.getLength(), ib.getType(), NULL);
    }
    else if (m_mode == RM_WIRED)
    { // else draw only the wireframe
      if ((*model_it)->isCovered())
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifdef Q_OS_WIN
        // this did not do very well in experiments on Linux, so rather disable it there
        glEnable(GL_POLYGON_OFFSET_LINE);
        //glPolygonOffset(0.25f, 0.25);
#endif
        glDrawElements(GL_QUADS, ib.getLength(), ib.getType(), NULL);

#ifdef Q_OS_WIN
        //glPolygonOffset(0.5f, 0.5f);
        glDisable(GL_POLYGON_OFFSET_LINE);
#endif
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
      else
      { // however the uncovered models are still rendered as normal
        int tex_ind = (*model_it)->getTextureIndex();
        tex_store.bindTexture(tex_ind);       
        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

        glDrawElements(GL_QUADS, ib.getLength(), ib.getType(), NULL);

        glColor4f(col_r, col_g, col_b, col_a);
        tex_store.unbindTexture();
      }
    }

    glDisable(GL_POLYGON_OFFSET_FILL);

    /* draw a selection box if selected */
    if ((*model_it)->isSelected())
    {
      glColor3f(m_sel_color.redF(), m_sel_color.greenF(), m_sel_color.blueF());
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      glDrawElements(GL_QUADS, ib.getLength(), ib.getType(), NULL);

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor4f(col_r, col_g, col_b, col_a);
    }

    ib.unbind();
    vb.unbind();

    ++model_it;
  }

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_POLYGON_SMOOTH);
  glDisable(GL_STENCIL_TEST);

  glPolygonOffset(0.0f, 0.0f);

  return;
}


/**
 */
void CRenderer::dimScreen(void)
{
  /* switch projection for dimming the screen */
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  /* pick a good black transparent color */
  glColor4f(0.0f, 0.0f, 0.0f, 0.75f);

  /* render a quad over the whole viewport */
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  /* and switch the projection back */
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  return;
}


/**
 */
void CRenderer::renderXYZAxes(void) const
{
  glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(1000.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 1000.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 1000.0f);
  glEnd();

  glColor3f(1.0, 1.0, 1.0);

  return;
}


/**
 */
void CRenderer::renderStencilBuffer(void)
{
  /* switch projection for background rendering */
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glColor4f(1.0f, 1.0f, 1.0f, 0.0f);        // white color

  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);      // draw all pixels where stencil buffer is not zero
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);   // do not modify stencil buffer
  glEnable(GL_STENCIL_TEST);                // enable stencil testing
  glDepthMask(GL_FALSE);                    // disable modyfying depth buffer

  /* render a quad over the whole viewport */
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
  glEnd();

  glDepthMask(GL_TRUE);                     // reenable modyfying depth buffer
  glDisable(GL_STENCIL_TEST);               // disable stencil testing
  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);      // draw all pixels where stencil buffer is not zero
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);   // do not modify stencil buffer

  /* and switch the projection back */
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  return;
}


/**
 */
const char *CRenderer::modeToStr(ERenderMode mode)
{
  static const char *strings[] = {
    "RM_NORMAL",
    "RM_UNCOVERED",
    "RM_COLORED",
    "RM_WIRED",
    "RM_NULL"
  };

  if ((mode < RM_NORMAL) || (mode > RM_NULL))
  {
    return "Unknown";
  }

  return strings[mode];
}


/**
 */
QDebug operator<<(QDebug debug, const CRenderer & renderer)
{
  debug.nospace() << "mode            : " << CRenderer::modeToStr(renderer.m_mode) << "\n"
                  << "viewport width  : " << renderer.m_w << "\n"
                  << "viewport height : " << renderer.m_h << "\n"
                  //<< "TrackBall       : " << (*renderer.m_track_ball)   << "\n"
                  << "selection color : " << renderer.m_sel_color       << "\n";

  return debug.maybeSpace();
}

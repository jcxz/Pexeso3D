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
 * Definition of renderer class
 */

#ifndef CRENDERER_H
#define CRENDERER_H

#include <QColor>


class CTrackBall;
class CScene;

/**
 * This class represents a model renderer.
 * It provides various rendering functions
 * and options to alter the rendering process.
 */
class CRenderer
{
  public:
    /** this enumeration defines how the scene will be rendered */
    enum ERenderMode {
      RM_NORMAL = 0,            /// render models covered or uncovred based on their state
      RM_UNCOVERED,             /// render allways all models uncovered
      RM_COLORED,               /// do not render with texture but use varying colors
      RM_GREYED,                /// use varying degrees of shade to render the models
      RM_WIRED,                 /// render the models in scene only as wireframes
      RM_NULL                   /// suppress rendering
    };

    static const QColor DEF_SELECTION_COLOR;  // default selection color

  public:
    explicit CRenderer(ERenderMode mode = RM_NORMAL)
      : m_mode(mode),
        m_w(0),
        m_h(0),
        m_track_ball(0),
        m_sel_color(DEF_SELECTION_COLOR)
    {
    }

    /**
     * This method will return the actual rendering mode
     */
    ERenderMode getMode(void) const
    {
      return m_mode;
    }

    /**
     * This method will set the rendering mode
     */
    void setMode(ERenderMode mode)
    {
      m_mode = mode;
      return;
    }

    /**
     * This method will switch to the next rendering mode
     */
    void toggleNextMode(void)
    {
      m_mode = CRenderer::ERenderMode((m_mode + 1) % CRenderer::RM_NULL);
      return;
    }

    /**
     * This method will switch to the previous rendering mode
     */
    void togglePrevMode(void)
    {
      m_mode = CRenderer::ERenderMode((m_mode + CRenderer::RM_NULL - 1) % CRenderer::RM_NULL);
      return;
    }

    /**
     * This method will return the currently used track ball
     */
    CTrackBall *getTrackBall(void) const
    {
      return m_track_ball;
    }

    /**
     * This method will set the trackball to be used for rendering
     */
    void setTrackBall(CTrackBall *track_ball)
    {
      m_track_ball = track_ball;
      return;
    }

    /**
     * This method will return the color that is used
     * to render selected object
     */
    const QColor & getSelectionColor(void) const
    {
      return m_sel_color;
    }

    /**
     * This method will set the color of selection rectangle
     */
    void setSelectionColor(const QColor & color)
    {
      m_sel_color = color;
      return;
    }

    /**
     * This method is used to do additional OpenGL initialisation
     *
     * @param w window width to set up viewport
     * @param h window height to set up viewport
     *
     * @param return true on success, false otherwise
     */
    bool init(int w, int h);

    /**
     * Reset all the state changed by renderer
     */
    bool deinit(void);

    /**
     * This method will resize the viewport used by renderer
     * according to given widow size.
     */
    void resize(int w, int h);

    /**
     * This method will render a background
     *
     * @param scene the scene tha contains the background
     */
    void renderBackground(const CScene & scene);

    /**
     * This method will render a scene
     */
    void renderScene(const CScene & scene);

    /**
     * This method will render a dimmed screen effect (liek with javascrip)
     * on top of the current scene
     */
    void dimScreen(void);

    /**
     * This method will draw x, y, z coordinate axes
     */
    void renderXYZAxes(void) const;   

    /**
     * This method will display the contents of stencil buffer
     */
    void renderStencilBuffer(void);

    /**
     * This function will return a string representation of the mode constant
     */
    static const char *modeToStr(ERenderMode mode);

    /**
     * A friend function to display rendering settings
     */
    friend QDebug operator<<(QDebug debug, const CRenderer & renderer);

  private:
    ERenderMode m_mode;        /// the current rendering mode
    int m_w;                   /// viewport width
    int m_h;                   /// viewport height
    CTrackBall *m_track_ball;  /// this class holds view transformations
    QColor m_sel_color;        /// color of selected box
};

#endif

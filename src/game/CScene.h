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
 * This file defines the scene class
 */

#ifndef CSCENE_H
#define CSCENE_H

#include <QVector>
#include <QString>
#include <QColor>

#include "CBaseModel.h"



class QXmlStreamReader;
struct SVector3D;

/**
 * This structure holds meta information about the current scene
 */
struct SSceneInfo
{
  QString name;   /// contains the scene's name
  QString desc;   /// contains the scene's textual description
  QString path;   /// contains a path to the scene file
};


/**
 * This class will represent a container
 * for all models in the scene. It manages their loading
 * from disc and their lifetime management.
 */
class CScene
{
  public:
    /** An enumeration of errors raised during XML parsing */
    enum EErrorCode {
      ERR_OK = 0,               /// no error

      ERR_FILE_READ,            /// problem reading the input file
      //ERR_FILE_WRITE,         /// problem reading the input file

      ERR_SYN_MALFORMED,        /// QXmlStreamReader's NotWellFormedError
      ERR_SYN_END,              /// QXmlStreamReader's PreamatureEndOfDocument
      ERR_SYN_UNEXPECTED,       /// QXmlStreamReader's UnexpectedElementError

      ERR_VER,                  /// unsupported version of scene file
      ERR_ELEM_SCENE_TAG,       /// 'scene' tag was not found
      //ERR_ELEM_NAME_TAG,        /// 'name' tag was not found
      //ERR_ELEM_NAME_VALUE,      /// 'name' tag value was missing
      //ERR_ELEM_DESC_TAG,        /// 'desc' tag was not found
      //ERR_ELEM_DESC_VALUE,      /// 'desc' tag value was missing

      ERR_ELEM_BACKGROUND_TAG,
      ERR_ELEM_BACKGROUND_VALUE,
      ERR_LOAD_TEX_BACKGROUND,

      ERR_ELEM_COVER_TAG,       /// 'cover' tag was not found
      ERR_ELEM_COVER_VALUE,     /// 'cover' tag  value was missing
      ERR_LOAD_TEX_COVER,       /// failed to load cover texture
      ERR_ELEM_TEXTURES_TAG,    /// 'textures' tag was not found
      ERR_ELEM_TEXTURES_VALUE,  /// other than 'tex' tag was found between textures
      ERR_ELEM_TEX_TAG,         /// 'tex' tag was not found
      ERR_LOAD_TEX_REVERSE,     /// failed to load the cube underside texture
      ERR_ELEM_CUBES_TAG,       /// 'cubes' tag was not found
      ERR_ELEM_CUBES_ATTR,      /// side attribute missing
      ERR_ELEM_CUBES_VALUE,     /// other than 'cube' tag was found between cubes
      ERR_ELEM_CUBE_TAG,        /// 'cube' tag was not found
      ERR_ELEM_CUBE_ATTR,       /// cube model attribute was wrong
      ERR_LOAD_CUBE_MEM,        /// failed to allocate memory for cube model
      ERR_LOAD_CUBE_BUILD,      /// failed to build cube model geometry

      ERR_LIST_VER,             /// unsupported version of scene list file
      ERR_LIST_ELEM_LIST_TAG,   /// list tag of scene list file missing
      ERR_LIST_ELEM_LIST_VALUE,
      ERR_LIST_ELEM_SCENE_TAG,
      ERR_LIST_ELEM_NAME_TAG,
      ERR_LIST_ELEM_NAME_VALUE,
      ERR_LIST_ELEM_DESC_TAG,
      ERR_LIST_ELEM_DESC_VALUE,
      ERR_LIST_ELEM_PATH_TAG,
      ERR_LIST_ELEM_PATH_VALUE,

      ERR_UNKNOWN               /// an unspecified error
    };

    static const QColor DEF_WF_COLOR;      /// use white as default
    static const float DEF_WF_LINE_WIDTH;  /// default wirframe line width is 4.0f

  public:
    /** Model iterator types */
    typedef QVector<CBaseModel *>::iterator       CModelIter;
    typedef QVector<CBaseModel *>::const_iterator CConstModelIter;

  public:
    /**
     * Default constructor
     */
    CScene(void)
      : m_error(ERR_OK),
        m_cover_tex_ind(CTextureStore::INVALID_INDEX),
        m_background_tex_ind(CTextureStore::INVALID_INDEX),
        m_wf_color(DEF_WF_COLOR),
        m_wf_line_width(DEF_WF_LINE_WIDTH),
        m_bbox(),
        m_tex_store(),
        m_models()
    { }

    /**
     * Destructor
     */
    ~CScene(void)
    {
      clear();
    }

    /**
     * This function will return an error code caused by reading XML
     */
    EErrorCode getError(void) const
    {
      return m_error;
    }

    /**
     * This method wil return an string for the error code that occured
     */
    QString getErrorString(void) const
    {
      return errorToString(m_error);
    }

    /**
     * This function wil return scene's bounding box
     */
    const CBBox & getBBox(void) const
    {
      return m_bbox;
    }

    /**
     * This method will return the color that has to be used
     * for drawing the wireframe color outline
     */
    const QColor & getWFColor(void) const
    {
      return m_wf_color;
    }

    /**
     * This function will return the width of the wireframe lines
     */
    const float getWFLineWidth(void) const
    {
      return m_wf_line_width;
    }

    /**
     * This method will return a vector with models from the scene
     */
    CConstModelIter getModelIterBegin(void) const
    {
      return m_models.begin();
    }

    /**
     * This method will return a vector with models from the scene
     */
    CModelIter getModelIterBegin(void)
    {
      return m_models.begin();
    }

    /**
     * This method will return a vector with models from the scene
     */
    CConstModelIter getModelIterEnd(void) const
    {
      return m_models.end();
    }

    /**
     * This method will return a vector with models from the scene
     */
    CModelIter getModelIterEnd(void)
    {
      return m_models.end();
    }

    /**
     * This method will return the number of models in the scene
     */
    int getModelCount(void) const
    {
      return m_models.size();
    }

    /**
     * This method will return the pointer to the vector of all models
     */
    QVector<CBaseModel *> *getModelVectorPointer(void)
    {
      return &m_models;
    }
    
    /**
     * This method will return a texture store with all the textures
     * from the scene
     */
    const CTextureStore & getTextures(void) const
    {
      return m_tex_store;
    }

    /**
     * This method will return the index of texture that should be used
     * as cover texture for pexeso cubes
     */
    int getCoverTextureIndex(void) const
    {
      return m_cover_tex_ind;
    }

    /**
     * This method will return the index of texture that should be used
     * as background texture
     */
    int getBackgroundTextureIndex(void) const
    {
      return m_background_tex_ind;
    }

    /**
     * This function returns a pointer to the model that was selected
     */
    // TODO: tuna by bolo mozno lepsie ak by sa predavala referencia na CCamera
    CBaseModel *calcIntersection(int x, int y) const;

    /**
     * This function will clear the contents of the scene and reset it
     */
    void clear(void);

    /**
     * This function will load the scene from file and randomly assign
     * back texture to every model
     */
    bool loadFromXML(const QString & filename);

    /**
     * This method will save the scene to an XML file
     */
    bool saveToXML(const QString & filename);

    /**
     * This method will randomly assign textures to models
     */
    void shuffleTextures(void);  // toot dat mozno ako private

    /**
     * This method will load a list of available scene's
     * from a given scene list. This function is made static
     * for convenience.
     *
     * @param path a path to the scene list file
     * @param list a pointer to vector that will be filled with scene information
     * @param simplify an option whether to remove excesive whitespace or not
     *
     * @return ERR_OK on success, an error code of operation on failure
     *         on failure list contents are undefined
     */
    static EErrorCode loadSceneList(const QString & path, QVector<SSceneInfo> *list, bool simplify = false);

    /**
     * This function will convert a given error code to an error string
     *
     * @param err an error to be coverted
     *
     * @return an error message string
     */
    static const char *errorToString(EErrorCode err);

    /**
     * A friend function to display loaded contents
     */
    friend QDebug operator<<(QDebug debug, const CScene & scene);

  private:
    /**
     * Functions to load the Scene XML file contents
     */
    EErrorCode loadContents(QXmlStreamReader & reader);
    EErrorCode loadMetaData(QXmlStreamReader & reader);
    EErrorCode loadTextures(QXmlStreamReader & reader);
    EErrorCode loadModels(QXmlStreamReader & reader);

    /**
     * Functions to read the XML scene list file contenst
     */
    static EErrorCode loadSceneListContents(QXmlStreamReader & reader, QVector<SSceneInfo> *list);
    static EErrorCode loadSceneListData(QXmlStreamReader & reader, QVector<SSceneInfo> *list);

  private:
    EErrorCode m_error;              /// the error while reading/writing XML
    int m_cover_tex_ind;             /// index of cover texture
    int m_background_tex_ind;        /// an index of background texture
    float m_wf_line_width;           /// a width of the line that will be used for drawing wireframe
    QColor m_wf_color;               /// the color of the wireframe
    CBBox m_bbox;                    /// scene's bounding box
    CTextureStore m_tex_store;       /// a storage of textures
    QVector<CBaseModel *> m_models;  /// a list of models in the scene
};

#endif

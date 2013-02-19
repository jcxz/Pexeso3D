/*
 * Copyright (C) 2012-2013 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
 *                         Josef Rudolf <xrudol04@stud.fit.vutbr.cz>
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

#include <QDebug>
#include <QXmlStreamReader>
#include <QFile>
#include <ctime>
#include <cstdlib>

#include "Global.h"
#include "CScene.h"
#include "CCubeModel.h"



/* initialize constants */
const QColor CScene::DEF_WF_COLOR = QColor(0xFF, 0xFF, 0xFF);  // use white as default wireframe color
const float CScene::DEF_WF_LINE_WIDTH = 4.0f;                  // use 4.0f as default wireframe line width



/**
 */
CBaseModel *CScene::calcIntersection(int x, int y) const
{
  GLint viewport[4];  // viewport (i.e window size)
  GLint index = 0;    // index on position [x, y]

  /* read out the stencil value */
  glGetIntegerv(GL_VIEWPORT, viewport);
  glReadPixels(x, viewport[3] - y, 1, 1, GL_STENCIL_INDEX, GL_INT, &index);

  qDebug() << "x == " << x << "y == " << y;
  qDebug() << viewport[0] << viewport[1] << viewport[2] << viewport[3];
  qDebug() << "index == " << index;

  /* index 0 is background */
  if (index == 0)
  {
    return NULL;
  }

  /* run through all the objects */
  for (CConstModelIter it = m_models.begin(); it != m_models.end(); ++it)
  { // maybe optimize out the test on m_models.end()
    // the index should allways be found
    if ((*it)->getID() == index)
    {
      return *it;
    }
  }

  return NULL;
}


/**
 */
void CScene::shuffleTextures(void)
{
  /* check on preconditions */
  if (m_tex_store.getSize() < 3)
  {
    qWarning() << PEXESO_FUNC
               << ": There must be at least a background and cover texture and one puzzle";
    return;
  }

  if (m_models.size() != ((m_tex_store.getSize() - 2) * 2))
  {
    qWarning() << PEXESO_FUNC
               << ": The number of models must equal double the number of textures";
    return;
  }

  /* initialize random number generator */
  srand(time(NULL));

  /* shuffle the textures */
  for (int i = m_models.size() - 1; i >= 0; --i)
  {
    /* pick a random index from range <0, i> */
    int index = (double(rand()) / double(RAND_MAX)) * i;

    /* assign the texture index to a model */
    m_models[index]->setTextureIndex((i % (m_tex_store.getSize() - 2)) + 2);

    /* swap the model with random index for the last unindexed one */
    CBaseModel *tmp = m_models[i];
    m_models[i] = m_models[index];
    m_models[index] = tmp;
  }

  return;
}


/**
 */
void CScene::clear(void)
{
  /* free all models */
  for (CModelIter it = m_models.begin(); it != m_models.end(); ++it)
  {
    delete *it;
  }

  /* clear all the member data */
  m_models.clear();
  m_tex_store.clear();
  m_cover_tex_ind = CTextureStore::INVALID_INDEX;
  m_background_tex_ind = CTextureStore::INVALID_INDEX;
  m_error = ERR_OK;

  return;
}


/**
 */
bool CScene::loadFromXML(const QString & filename)
{
  QFile file(filename);
  QXmlStreamReader reader(&file);

  /* reset the scene */
  clear();

  /* open the scene file */
  if (!file.open(QIODevice::ReadOnly))
  {
    m_error = ERR_FILE_READ;
    qDebug() << getErrorString();
    return false;
  }

  /* load the contents of the XML file*/
  m_error = loadContents(reader);
  if (m_error != ERR_OK)
  {
    qDebug() << "loadContents failed " << getErrorString();
    clear();  // clear the scene on error
    return false;
  }

  /* make sure that the number of models is twice the number of textures,
     excluding the cover texture */
  if (((m_tex_store.getSize() - 2) * 2) != m_models.size())
  {
    qDebug() << "The number of models (" << m_models.size()
            << ") is not exactly twice the number of underside textures ("
            << (m_tex_store.getSize() - 1) << ") ";
    clear();  // clear the scene on error
    return false;
  }

  /* compute scene's bounding box and set model id's */
  CModelIter it = m_models.begin();
  CModelIter it_end = m_models.end();
  GLint id = 1;

  m_bbox.reset();

  while (it != it_end)
  {
    m_bbox.adjust((*it)->getBBox());
    (*it)->setID(id);
    ++id;
    ++it;
  }

  /* initialize models with random textures */
  shuffleTextures();

  return true;
}


/**
 */
CScene::EErrorCode CScene::loadContents(QXmlStreamReader & reader)
{
  /* check on the document start */
  QXmlStreamReader::TokenType token = reader.readNext();
  if (token != QXmlStreamReader::StartDocument)
  {
    qWarning() << "Document start missing";
  }

  /* read the root tag and check file version */
  if ((!reader.readNextStartElement()) || (reader.name() != "scene"))
  {
    return ERR_ELEM_SCENE_TAG;
  }

  if (reader.attributes().value("ver") != "1")
  {
    return ERR_VER;
  }

  /* load scene's meta data */
  EErrorCode ecode = loadMetaData(reader);
  if (ecode != ERR_OK)
  {
    return ecode;
  }

  /* load scene's textures */
  ecode = loadTextures(reader);
  if (ecode != ERR_OK)
  {
    return ecode;
  }

  /* load scene's models */
  ecode = loadModels(reader);
  if (ecode != ERR_OK)
  {
    return ecode;
  }

  /* check on the document end */
  if (token != QXmlStreamReader::EndDocument)
  {
    qWarning() << "End of document missing";
  }

  /* check on QXmlStreamReader errors */
  QXmlStreamReader::Error xml_err = reader.error();

  if (xml_err == QXmlStreamReader::NotWellFormedError)
  {
    return ERR_SYN_MALFORMED;
  }
  
  if (xml_err == QXmlStreamReader::PrematureEndOfDocumentError)
  {
    return ERR_SYN_END;
  }

  if (xml_err == QXmlStreamReader::UnexpectedElementError)
  {
    return ERR_SYN_UNEXPECTED;
  }

  return ERR_OK;
}


/**
 */
CScene::EErrorCode CScene::loadMetaData(QXmlStreamReader & reader)
{
#if 0
  /* process name tag */
  if ((!reader.readNextStartElement()) || (reader.name() != "name"))
  {
    return ERR_ELEM_NAME_TAG;
  }

  m_name = reader.readElementText();
  if (reader.hasError())
  {
    return ERR_ELEM_NAME_VALUE;
  }

  /* process desc tag */
  if ((!reader.readNextStartElement()) || (reader.name() != "desc"))
  {
    return ERR_ELEM_DESC_TAG;
  }

  m_desc = reader.readElementText();
  if (reader.hasError())
  {
    return ERR_ELEM_DESC_VALUE;
  }
#endif
  /* process wireframecolor, the QColor is in #AARRGGBB format */
  if ((reader.readNextStartElement()) && (reader.name() == "wfcolor"))
  {
    bool ok = true;
    m_wf_color = reader.readElementText().toUInt(&ok);
    if (!ok)
    {
      m_wf_color = DEF_WF_COLOR;
      qDebug() << "Warning: Wireframe color setting incorrect";
    }
    reader.readNextStartElement();  // read the next element after
  }
  else
  {
    m_wf_color = DEF_WF_COLOR;
    qDebug() << "No wireframe color set. Using default color.";
  }

  /* read the wireframe width */
  if (reader.name() == "wfwidth")
  {
    bool ok = true;
    m_wf_line_width = reader.readElementText().toFloat(&ok);
    if (!ok)
    {
      m_wf_line_width = DEF_WF_LINE_WIDTH;
      qDebug() << "Warning: Wireframe color setting incorrect";
    }
    reader.readNextStartElement();  // read the next element after
  }
  else
  {
    m_wf_line_width = DEF_WF_LINE_WIDTH;
    qDebug() << "No wireframe line width set. Using default line width.";
  }

  /* process background tag */
  //if ((!reader.readNextStartElement()) || (reader.name() != "background"))
  if (reader.name() != "background")
  {
    return ERR_ELEM_BACKGROUND_TAG;
  }

  QString path = reader.readElementText();
  if (reader.hasError())
  {
    return ERR_ELEM_BACKGROUND_VALUE;
  }

  m_background_tex_ind = m_tex_store.loadTexture(path);
  if (m_background_tex_ind == CTextureStore::INVALID_INDEX)
  {
    return ERR_LOAD_TEX_BACKGROUND;
  }

  /* process cover tag */
  if ((!reader.readNextStartElement()) || (reader.name() != "cover"))
  {
    return ERR_ELEM_COVER_TAG;
  }

  path = reader.readElementText();
  if (reader.hasError())
  {
    return ERR_ELEM_COVER_VALUE;
  }

  m_cover_tex_ind = m_tex_store.loadTexture(path);
  if (m_cover_tex_ind == CTextureStore::INVALID_INDEX)
  {
    return ERR_LOAD_TEX_COVER;
  }

  return ERR_OK;
}


/**
 */
CScene::EErrorCode CScene::loadTextures(QXmlStreamReader & reader)
{
  /* process textures tag */
  if ((!reader.readNextStartElement()) || (reader.name() != "textures"))
  {
    return ERR_ELEM_TEXTURES_TAG;
  }

  QXmlStreamAttributes attrs;

  /* load the textures */
  while (1)
  {
    /* get the next texture */
    if (!reader.readNextStartElement())
    {
      if ((reader.name() != "textures") || (reader.hasError()))
      {
        return ERR_ELEM_TEXTURES_VALUE;
      }

      return ERR_OK;
    }

    if (reader.name() != "tex")
    {
      return ERR_ELEM_TEX_TAG;
    }

    /* get textures attributes and load it */
    attrs = reader.attributes();
    int tex_id = m_tex_store.loadTexture(attrs.value("path").toString());
    if (tex_id == CTextureStore::INVALID_INDEX)
    {
      return ERR_LOAD_TEX_REVERSE;
    }

    /* skip end elements */
    reader.readElementText();
  }

  return ERR_OK;
}


/**
 */
CScene::EErrorCode CScene::loadModels(QXmlStreamReader & reader)
{
  /* process cubes tag */
  if ((!reader.readNextStartElement()) || (reader.name() != "cubes"))
  {
    return ERR_ELEM_CUBES_TAG;
  }

  Maths::SVector3D pos;
  QXmlStreamAttributes attrs = reader.attributes();
  bool ok = false;
  double side = attrs.value("side").toString().toDouble(&ok);

  if (!ok)
  {
    return ERR_ELEM_CUBES_ATTR;
  }

  /* load cubes */
  while (1)
  {
    /* get the next cube */
    if (!reader.readNextStartElement())
    {
      if ((reader.name() != "cubes") || (reader.hasError()))
      {
        return ERR_ELEM_CUBES_VALUE;
      }

      return ERR_OK;
    }

    if (reader.name() != "cube")
    {
      return ERR_ELEM_CUBE_TAG;
    }

    /* get cube's attributes */
    attrs = reader.attributes();
  
    pos.x = attrs.value("x").toString().toDouble(&ok);
    if (!ok)
    {
      return ERR_ELEM_CUBE_ATTR;
    }

    pos.y = attrs.value("y").toString().toDouble(&ok);
    if (!ok)
    {
      return ERR_ELEM_CUBE_ATTR;
    }

    pos.z = attrs.value("z").toString().toDouble(&ok);
    if (!ok)
    {
      return ERR_ELEM_CUBE_ATTR;
    }

    /* create the cube model */
    CCubeModel *model = new (std::nothrow) CCubeModel;
    if (model == NULL)
    {
      return ERR_LOAD_CUBE_MEM;
    }

    if (!model->build(pos, side))
    {
      return ERR_LOAD_CUBE_BUILD;
    }

    /* add the cube model to models array */
    m_models.append(model);

    /* skip end elements */
    reader.readElementText();
  }

  return ERR_OK;
}


/**
 */
bool CScene::saveToXML(const QString & filename)
{
  PEXESO_UNUSED(filename);
  return true;
}


/**
 */
CScene::EErrorCode CScene::loadSceneList(const QString & path, QVector<SSceneInfo> *list, bool simplify)
{
  PEXESO_ASSERT(list != NULL);

  QFile file(path);
  QXmlStreamReader reader(&file);

  /* clear the list */
  list->clear();

  /* open the scene file */
  if (!file.open(QIODevice::ReadOnly))
  {
    qDebug() << file.errorString();
    return ERR_FILE_READ;
  }

  EErrorCode error = loadSceneListContents(reader, list);
  if (error != ERR_OK)
  {
    return error;
  }

  /* replace all whitespace characters if requested */
  if (!simplify)
  {
    return ERR_OK;
  }

  for (QVector<SSceneInfo>::iterator it = list->begin(); it != list->end(); ++it)
  {
    it->name = it->name.simplified();
    it->desc = it->desc.simplified();
  }

  return ERR_OK;
}


/**
 */
CScene::EErrorCode CScene::loadSceneListContents(QXmlStreamReader & reader, QVector<SSceneInfo> *list)
{
  /* check on the document start */
  QXmlStreamReader::TokenType token = reader.readNext();
  if (token != QXmlStreamReader::StartDocument)
  {
    qWarning() << "Document start missing";
  }

  /* read the root tag and check file version */
  if ((!reader.readNextStartElement()) || (reader.name() != "list"))
  {
    return ERR_LIST_ELEM_LIST_TAG;
  }

  if (reader.attributes().value("ver") != "1")
  {
    return ERR_LIST_VER;
  }

  /* load scene's meta data */
  EErrorCode ecode = loadSceneListData(reader, list);
  if (ecode != ERR_OK)
  {
    return ecode;
  }

  /* check on the document end */
  if (token != QXmlStreamReader::EndDocument)
  {
    qWarning() << "End of document missing";
  }

  /* check on QXmlStreamReader errors */
  QXmlStreamReader::Error xml_err = reader.error();

  if (xml_err == QXmlStreamReader::NotWellFormedError)
  {
    return ERR_SYN_MALFORMED;
  }

  if (xml_err == QXmlStreamReader::PrematureEndOfDocumentError)
  {
    return ERR_SYN_END;
  }

  if (xml_err == QXmlStreamReader::UnexpectedElementError)
  {
    return ERR_SYN_UNEXPECTED;
  }

  return ERR_OK;
}


/**
 */
CScene::EErrorCode CScene::loadSceneListData(QXmlStreamReader & reader, QVector<SSceneInfo> *list)
{
  SSceneInfo info;

  /* load the textures */
  while (1)
  {
    if (reader.readNextStartElement())
    {
      if (reader.name() != "scene")
      {
        return ERR_LIST_ELEM_SCENE_TAG;
      }

      /* process name tag */
      if ((!reader.readNextStartElement()) || (reader.name() != "name"))
      {
        return ERR_LIST_ELEM_NAME_TAG;
      }

      info.name = reader.readElementText();
      if (reader.hasError())
      {
        return ERR_LIST_ELEM_NAME_VALUE;
      }

      /* process desc tag */
      if ((!reader.readNextStartElement()) || (reader.name() != "desc"))
      {
        return ERR_LIST_ELEM_DESC_TAG;
      }

      info.desc = reader.readElementText();
      if (reader.hasError())
      {
        return ERR_LIST_ELEM_DESC_VALUE;
      }

      /* process path tag */
      if ((!reader.readNextStartElement()) || (reader.name() != "path"))
      {
        return ERR_LIST_ELEM_PATH_TAG;
      }

      info.path = reader.readElementText();
      if (reader.hasError())
      {
        return ERR_LIST_ELEM_PATH_VALUE;
      }

      /* add scene info to the list */
      list->append(info);
    }
    else
    {
      if (reader.hasError())
      {
        return ERR_LIST_ELEM_LIST_VALUE;
      }
      else if (reader.name() == "list")
      {
        return ERR_OK;
      }
    }
  }

  return ERR_OK;
}


/**
 */
const char *CScene::errorToString(EErrorCode err)
{
  static const char *err_strings[] = {
    "ERR_OK",

    "ERR_FILE_READ",
    //ERR_FILE_WRITE,         /// problem reading the input file

    "ERR_SYN_MALFORMED",
    "ERR_SYN_END",
    "ERR_SYN_UNEXPECTED",

    "ERR_VER",
    "ERR_ELEM_SCENE_TAG",

    //"ERR_ELEM_NAME_TAG",
    //"ERR_ELEM_NAME_VALUE",
    //"ERR_ELEM_DESC_TAG",
    //"ERR_ELEM_DESC_VALUE",

    "ERR_ELEM_BACKGROUND_TAG",
    "ERR_ELEM_BACKGROUND_VALUE",
    "ERR_LOAD_TEX_BACKGROUND",

    "ERR_ELEM_COVER_TAG",
    "ERR_ELEM_COVER_VALUE",
    "ERR_LOAD_TEX_COVER",
    "ERR_ELEM_TEXTURES_TAG",
    "ERR_ELEM_TEXTURES_VALUE",
    "ERR_ELEM_TEX_TAG",
    "ERR_LOAD_TEX_REVERSE",
    "ERR_ELEM_CUBES_TAG",
    "ERR_ELEM_CUBES_ATTR",
    "ERR_ELEM_CUBES_VALUE",
    "ERR_ELEM_CUBE_TAG",
    "ERR_ELEM_CUBE_ATTR",
    "ERR_LOAD_CUBE_MEM",
    "ERR_LOAD_CUBE_BUILD",

    "ERR_LIST_VER",
    "ERR_LIST_ELEM_LIST_TAG",
    "ERR_LIST_ELEM_LIST_VALUE",
    "ERR_LIST_ELEM_SCENE_TAG",
    "ERR_LIST_ELEM_NAME_TAG",
    "ERR_LIST_ELEM_NAME_VALUE",
    "ERR_LIST_ELEM_DESC_TAG",
    "ERR_LIST_ELEM_DESC_VALUE",
    "ERR_LIST_ELEM_PATH_TAG",
    "ERR_LIST_ELEM_PATH_VALUE",

    "ERR_UNKNOWN"
  };

  if ((err < ERR_OK) || (err > ERR_UNKNOWN))
  {
    return err_strings[ERR_UNKNOWN];
  }

  return err_strings[err];
}


/**
 */
QDebug operator<<(QDebug debug, const CScene & scene)
{
  debug.nospace() << "bounding box  : " << scene.m_bbox                << "\n"
                  << "cover tex ind : " << scene.m_cover_tex_ind       << "\n"
                  << "cover tex ind : " << scene.m_background_tex_ind  << "\n"
                  << "textures num  : " << scene.m_tex_store.getSize() << "\n"
                  << "models num    : " << scene.m_models.size()       << "\n"
                  << "models:       \n";

  CScene::CConstModelIter it;
  for (it = scene.m_models.begin(); it != scene.m_models.end(); ++it)
  {
    debug.nospace() << **it << "\n";
  }

  return debug.maybeSpace();
}

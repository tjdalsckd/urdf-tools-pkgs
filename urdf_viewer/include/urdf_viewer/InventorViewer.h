/**
    Copyright (C) 2016 Jennifer Buehler

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**/

#ifndef URDF_VIEWER_INVENTORVIEWER_H
#define URDF_VIEWER_INVENTORVIEWER_H
// Copyright Jennifer Buehler

#include <Inventor/Qt/SoQt.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoMaterial.h>

#include <Eigen/Geometry>

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace urdf_viewer
{
/**
 * \brief Runs an inventor viewer.
 * Allows to click points on the model, which
 * can be handled in different ways by subclasses.
 *
 * \author Jennifer Buehler
 * \date March 2016
 */
class InventorViewer
{
public:

    /**
     * \param _faces_ccw faces are to be treated as counter-clockwise. Needed for normal calculations.
     */
    explicit InventorViewer(bool _faces_ccw = true);

    InventorViewer(const InventorViewer& o);
    ~InventorViewer();


    /**
     * Initialize Inventor and Qt
     * TODO: For some reason I haven't yet further investigated, this has to be called **after**
     * functions as Urdf2Inventor::loadAndGetAsInventor(), or it won't work. Find out why, and fix it.
     */
    void init(const char * windowName = "InventorViewer", float bck_r=0.3, float bck_g=0.3, float bck_b=0.3);

    /**
     * Load a model into the viewer
     */
    void loadModel(SoNode * model);

    /**
     * Load a model from an inventor file
     */
    bool loadModel(const std::string& filename);

    void runViewer();

protected:

    /**
     * For subclasses: Called when user clicks on part of the model
     */
    virtual void onClickModel(const SoPickedPoint * pickPoint) {}
    /**
     * For subclasses: on any mouse button click
     */
    virtual void onMouseBtnClick(SoEventCallback *pNode) {}

    /**
     * Calculates the correct face normal of the pick point.
     * \param shapeIdx output: the index in the \e pick path at which the actual shape resides. 
     *      Normal coordinates are in the frame of this shape.
     */
    static bool computeCorrectFaceNormal(const SoPickedPoint * pick, bool ccw_face, Eigen::Vector3d& normal, int& shapeIdx);

    /**
     * Helper function which can be used to determine a URDF link name from a picked path. Links are found
     * based on their name (which is formatted *_visual_<number>_<name>). The last link found in the path
     * is returned. 
     * \param linkName output: name of the link
     * \param visualNum output: number of the link in the name
     * \param pathIdx output: index the link has in the path.
     */
    static SoNode * getLinkDesc(const SoPath * path, std::string& linkName, int& visualNum, int& pathIdx);

    SoQtExaminerViewer * getViewer()
    {
        return viewer;
    }

    SoSelection * getRoot()
    {
        return root;
    }

    bool isFacesCCW() const
    {
        return faces_ccw;
    }

private:

    static void mouseBtnCB(void *userData, SoEventCallback *pNode);

    QWidget * viewWindow;
    SoQtExaminerViewer * viewer;
    bool faces_ccw;
    SoSelection * root;
};

}  //  namespace urdf_viewer
#endif   // URDF_VIEWER_INVENTORVIEWER_H

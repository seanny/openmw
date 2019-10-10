#ifndef CSV_RENDER_TERRAINSHAPEMODE_H
#define CSV_RENDER_TERRAINSHAPEMODE_H

#include "editmode.hpp"

#include <string>
#include <memory>

#include <QWidget>
#include <QEvent>

#ifndef Q_MOC_RUN
#include "../../model/world/data.hpp"
#include "../../model/world/land.hpp"
#include "../../model/doc/document.hpp"
#include "../../model/world/commands.hpp"
#include "../../model/world/idtable.hpp"
#include "../../model/world/landtexture.hpp"
#include "../widget/brushshapes.hpp"
#endif

#include "terrainselection.hpp"

namespace CSVWidget
{
    class SceneToolShapeBrush;
}

namespace CSVRender
{
    class PagedWorldspaceWidget;

    /// \brief EditMode for handling the terrain shape editing
    class TerrainShapeMode : public EditMode
    {
        Q_OBJECT

        public:

            enum InteractionType
            {
                InteractionType_PrimaryEdit,
                InteractionType_PrimarySelect,
                InteractionType_SecondaryEdit,
                InteractionType_SecondarySelect,
                InteractionType_None
            };

            enum ShapeEditTool
            {
                ShapeEditTool_Drag = 0,
                ShapeEditTool_PaintToRaise = 1,
                ShapeEditTool_PaintToLower = 2,
                ShapeEditTool_Smooth = 3,
                ShapeEditTool_Flatten = 4
            };

            /// Editmode for terrain shape grid
            TerrainShapeMode(WorldspaceWidget*, osg::Group* parentNode, QWidget* parent = nullptr);

            void primaryOpenPressed (const WorldspaceHitResult& hit) final;

            /// Create single command for one-click shape editing
            void primaryEditPressed (const WorldspaceHitResult& hit) final;

            /// Open brush settings window
            void primarySelectPressed(const WorldspaceHitResult&) final;

            void secondarySelectPressed(const WorldspaceHitResult&) final;

            void activate(CSVWidget::SceneToolbar*) final;
            void deactivate(CSVWidget::SceneToolbar*) final;

            /// Start shape editing command macro
            bool primaryEditStartDrag (const QPoint& pos) final;

            bool secondaryEditStartDrag (const QPoint& pos) final;
            bool primarySelectStartDrag (const QPoint& pos) final;
            bool secondarySelectStartDrag (const QPoint& pos) final;

            /// Handle shape edit behavior during dragging
            void drag (const QPoint& pos, int diffX, int diffY, double speedFactor) final;

            /// End shape editing command macro
            void dragCompleted(const QPoint& pos) final;

            /// Cancel shape editing, and reset all pending changes
            void dragAborted() final;

            void dragWheel (int diff, double speedFactor) final;
            void dragMoveEvent (QDragMoveEvent *event) final;

        private:
            /// Move pending alteredHeights changes to omwgame/omeaddon -data
            void applyTerrainEditChanges();

            /// Handle brush mechanics for shape editing
            void editTerrainShapeGrid (const std::pair<int, int>& vertexCoords, bool dragOperation);

            /// set the target height for flatten tool
            void setFlattenToolTargetHeight(const WorldspaceHitResult& hit);

            /// Do a single height alteration for transient shape edit map
            void alterHeight(const CSMWorld::CellCoordinates& cellCoords, int inCellX, int inCellY, float alteredHeight, bool useTool = true);

            /// Do a single smoothing height alteration for transient shape edit map
            void smoothHeight(const CSMWorld::CellCoordinates& cellCoords, int inCellX, int inCellY, int toolStrength);

            /// Do a single flattening height alteration for transient shape edit map
            void flattenHeight(const CSMWorld::CellCoordinates& cellCoords, int inCellX, int inCellY, int toolStrength, int targetHeight);

            /// Get altered height values around one vertex
            void updateKeyHeightValues(const CSMWorld::CellCoordinates& cellCoords, int inCellX, int inCellY, float* thisHeight,
                float* thisAlteredHeight, float* leftHeight, float* leftAlteredHeight, float* upHeight, float* upAlteredHeight,
                float* rightHeight, float* rightAlteredHeight, float* downHeight, float* downAlteredHeight);

            ///Limit steepness based on either X or Y and return false if steepness is limited
            void compareAndLimit(const CSMWorld::CellCoordinates& cellCoords, int inCellX, int inCellY, float* limitedAlteredHeightXAxis,
                float* limitedAlteredHeightYAxis, bool* steepnessIsWithinLimits);

            /// Check that the edit doesn't break save format limits, fix if necessary, return true if slope steepness is within limits
            bool limitAlteredHeights(const CSMWorld::CellCoordinates& cellCoords, bool reverseMode = false);

            /// Handle brush mechanics for terrain shape selection
            void selectTerrainShapes (const std::pair<int, int>& vertexCoords, unsigned char selectMode, bool dragOperation);

            /// Push terrain shape edits to command macro
            void pushEditToCommand (const CSMWorld::LandHeightsColumn::DataType& newLandGrid, CSMDoc::Document& document,
                CSMWorld::IdTable& landTable, const std::string& cellId);

            /// Push land normals edits to command macro
            void pushNormalsEditToCommand(const CSMWorld::LandNormalsColumn::DataType& newLandGrid, CSMDoc::Document& document,
                CSMWorld::IdTable& landTable, const std::string& cellId);

            /// Generate new land map LOD
            void pushLodToCommand(const CSMWorld::LandMapLodColumn::DataType& newLandMapLod, CSMDoc::Document& document,
                CSMWorld::IdTable& landTable, const std::string& cellId);

            /// Create new cell and land if needed
            bool allowLandShapeEditing(const std::string& textureFileName);

            std::string mCellId;
            std::string mBrushTexture;
            int mBrushSize;
            CSVWidget::BrushShape mBrushShape;
            std::vector<std::pair<int, int>> mCustomBrushShape;
            CSVWidget::SceneToolShapeBrush *mShapeBrushScenetool;
            int mDragMode;
            osg::Group* mParentNode;
            bool mIsEditing;
            std::unique_ptr<TerrainSelection> mTerrainShapeSelection;
            int mTotalDiffY;
            std::vector<CSMWorld::CellCoordinates> mAlteredCells;
            osg::Vec3d mEditingPos;
            int mShapeEditTool;
            int mShapeEditToolStrength;
            int mTargetHeight;

            PagedWorldspaceWidget& getPagedWorldspaceWidget();

        signals:
            void passBrushTexture(std::string brushTexture);

        public slots:
            void setBrushSize(int brushSize);
            void setBrushShape(CSVWidget::BrushShape brushShape);
            void setShapeEditTool(int shapeEditTool);
            void setShapeEditToolStrength(int shapeEditToolStrength);
    };
}


#endif

#ifndef H_ASSETINSPECTOR
#define H_ASSETINSPECTOR

#include "inspector.h"
#include "meshData.h"

class AssetInspector: public Inspector{

public:

    bool bKinectToolOpen;           //already running a kinect tool?
    bool bShowUntitled;

   class MeshTab: public Tab{

        public:

        MeshTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };

    class TextureTab: public Tab{

        public:

        TextureTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };

    class ActionTab: public Tab{

        public:

        ActionTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };

    class PrefabTab: public Tab{

        public:

        PrefabTab(TabbedListButton* m){mine=m;};
        virtual void assembleList();
        virtual void trigger(MsbObject* other);
    };


    AssetInspector();
    virtual ~AssetInspector();

    virtual void registerProperties();
    virtual void setup();

    virtual void createInspectorButtons();

    virtual void refreshList();

    virtual void trigger(MsbObject* other);

    virtual void openKinectTool(bool bHighZRes=false);
    virtual void closeKinectTool();
    virtual void importKinect(bool bHighZRes=false);

    virtual void create();
};
#endif // AssetInspector

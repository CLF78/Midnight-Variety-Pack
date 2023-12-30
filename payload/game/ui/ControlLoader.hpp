#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>

class ControlLoader {
public:
    ControlLoader(LayoutUIControl* control);
    ~ControlLoader();

    void load(const char* dirname, const char* filename, const char* variant, const char** anims);

    LayoutUIControl* control;
    void* ctrlFile;
    void* ctrlSubHeader1;
    void* ctrlSection1;
    void* ctrlSection2;
    void* ctrlSubHeader2;
    void* ctrlSection3;
    void* ctrlSection4;
    void* ctrlSection5;
    char* nameTable;
};
size_assert(ControlLoader, 0x28);

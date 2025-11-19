#include "akinator.h"

#define PATH_TO_LOGFILE "AKINATOR/ReportFiles/LogFile.log"
#define PATH_TO_DATA    "AKINATOR/src/data.txt"


int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    tree_t *akin_tree = {0};
    AkinCtor(&akin_tree);

    DataReader(PATH_TO_DATA, akin_tree);
    GenGraphs(akin_tree, __func__);

    Akinator(akin_tree);

    // GameStart();

    // if (mode == FIND_MODE)
    //     Akinator(akin_tree);
    // else if (mode == DEFINE_MODE)
    //     GetNodeDef(akin_tree);
    // else if (mode == CMP_MODE)
    //     NodesCompare(akin_tree);

    GenGraphs(akin_tree, __func__);

    DataUpdate(PATH_TO_DATA, akin_tree);

    AkinDtor(akin_tree);    
    GenHTML();

    LogFileCloser();

    return 0;
}
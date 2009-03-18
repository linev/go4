#ifndef TGO4DYNAMICLIST_H
#define TGO4DYNAMICLIST_H

#include "Rtypes.h"

class TFolder;
class TObject;
class TGo4TreeHistogramEntry;
class TGo4HistogramEntry;

/**
 * Aggregate which is responsible for the dynamically created analysis objects like histograms. Dynamic list may contain other
 * dynamic lists, which are processed iteratively.
 * @author J. Adamczewski
 * @since 1/2001
 */

class TGo4DynamicList {
  public:
    TGo4DynamicList() {}

    virtual ~TGo4DynamicList() {}

    static void ResetEntries(TFolder* folder);

    static void PrintEntries(TFolder* folder);

    static void CleanupPointerInEntries(TFolder* folder, TObject* obj);

    /** Iterates all entries of the list and processes the objects, depending on coordinate values and analysis condition.*/
    static void ProcessEntries(TFolder* folder, Bool_t processtrees, Int_t interval);

    static bool ProcessHEntry(TGo4HistogramEntry* hentry);

    static bool ProcessTEntry(TGo4TreeHistogramEntry* tentry, Bool_t processtrees, Int_t interval);
};

#endif //TGO4DYNAMICLIST_H

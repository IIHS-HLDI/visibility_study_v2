#include "FileSelectors.h"



/**
 */
std::optional<std::string> chooseProjectFile() {
    // auto-freeing memory
    /*std::string pathStr;*/
    NFD::UniquePath outPath;

    // prepare filters for the dialog
    nfdfilteritem_t filterItem[1] = {{"OGRE Project Files", "ogreproj"}};

    // show the dialog
    nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1);
    if (result == NFD_OKAY) {
        /*pathStr = outPath.get();*/
        return std::make_optional<std::string>(outPath.get());
        //std::cout << "Success!" << std::endl << outPath.get() << std::endl;
    } else if (result == NFD_CANCEL) {
        return std::nullopt;

    } else {
        return std::nullopt;
    }

    return std::nullopt;
}

/**
 * Opens a file save dialog for a single output CSV file
 *
 * returns the filepath string of user selected file or nullopt if user 
 * cancelled or there was an error
 *
 */
std::optional<std::string> chooseOutputCSV() {
    nfdchar_t* savePath;
    std::string savePathStr;

    // prepare filters for the dialog
    nfdfilteritem_t filterItem[1] = 
    {{"Comma-separated values file", 
         "csv"}};

    // show the dialog
    nfdresult_t result = 
        NFD_SaveDialog(
            &savePath, // pointer to save path string 
            filterItem, // filter items 
            1,  // number of filter items
            NULL, "");

    // Check result
    if (result == NFD_OKAY) { // if result is okay

        // convert savepath to string
        savePathStr = savePath;

        // Free the memory
        NFD_FreePath(savePath);

        // return string
        return std::make_optional(savePathStr);

    } else if (result == NFD_CANCEL) { // if user cancels dialog

        // No need to free here bc savePath is not allocated when a 
        // user cancels

        // return nullopt meaning that no filepath is given
        return std::nullopt;

    } else {
        // No need to free here bc savePath is not allocated when a 
        // user cancels
        
        // return nullopt meaning that no filepath is given
        return std::nullopt;
    }

}

/**
 * Opens file dialog for choosing project parent directory
 *
 * returns optional filepath if user selects file.
 * returns nullopt if user cancels or there is an error
 *
 */
std::optional<std::string> chooseParentDirectory() {

	NFD::UniquePath path;

	nfdresult_t result = NFD::PickFolder(path);
	if (result == NFD_OKAY) {
        return std::make_optional<std::string>(path.get());
        /*project->parentDirectory = outPath.get();*/
	}
	else if (result == NFD_CANCEL) {
        return std::nullopt;
	}
	else {
        return std::nullopt;
	}

}

/**
 * Open file chooser dialog for measurement image files
 *
 * Returns vector of filepath strings. if user cancels or there is an error,
 * returns an empty vector
 *
 *
 */
std::vector<std::string> chooseImageFiles() {

  std::vector<std::string> imageFilePaths;

  // auto-freeing memory
  NFD::UniquePathSet paths;

  // prepare filters for the dialog
  nfdfilteritem_t filterItem[1] = {{"Image Files", "jpg,jpeg,png"}};

  // show the dialog
  nfdresult_t result = NFD::OpenDialogMultiple(paths, filterItem, 1);
  if (result == NFD_OKAY) {

    nfdpathsetsize_t numPaths;
    NFD::PathSet::Count(paths, numPaths);

    for (nfdpathsetsize_t i = 0; i < numPaths; ++i) {
      NFD::UniquePathSetPath path;
      NFD::PathSet::GetPath(paths, i, path);
      imageFilePaths.push_back(path.get());
    }
  } else if (result == NFD_CANCEL) {
    return {};
  } else {
    return {};
  }
  return {};
}

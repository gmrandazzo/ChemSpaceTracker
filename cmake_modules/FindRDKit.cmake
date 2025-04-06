# FindRDKit.cmake
# Based on public domain script by NextMove Software (2013), modified.
# Try to find RDKit headers and libraries
# Defines:
#
#  RDKIT_FOUND - system has RDKit
#  RDKIT_INCLUDE_DIRS - the RDKit include directories (plural is preferred)
#  RDKIT_LIBRARIES - Link these to use RDKit (list of libraries)
#  RDKit::XXX targets (Optional, recommended for modern CMake)
#
# References:
#
#  http://nextmovesoftware.com/blog/2013/02/04/looking-for-a-c-cheminformatics-toolkit/
#  https://github.com/timvdm/MolDB/blob/master/cmake/modules/FindRDKit.cmake

include(FindPackageHandleStandardArgs)

if(RDKIT_INCLUDE_DIR AND NOT RDKIT_INCLUDE_DIRS)
  set(RDKIT_INCLUDE_DIRS ${RDKIT_INCLUDE_DIR})
endif()

if(RDKIT_INCLUDE_DIRS AND RDKIT_LIBRARIES)
  find_package_handle_standard_args(RDKit DEFAULT_MSG
                                    REQUIRED_VARS RDKIT_LIBRARIES RDKIT_INCLUDE_DIRS)
else()

  if(NOT RDKIT_INCLUDE_DIRS)
    set(RDKIT_INCLUDE_SEARCH_PATHS)
    if(DEFINED ENV{RDKIT_INCLUDE_DIR})
      list(APPEND RDKIT_INCLUDE_SEARCH_PATHS "$ENV{RDKIT_INCLUDE_DIR}")
    endif()
    if(DEFINED ENV{RDKIT_INCLUDE_PATH})
      list(APPEND RDKIT_INCLUDE_SEARCH_PATHS "$ENV{RDKIT_INCLUDE_PATH}")
    endif()
     if(DEFINED ENV{RDKIT_BASE})
      list(APPEND RDKIT_INCLUDE_SEARCH_PATHS "$ENV{RDKIT_BASE}/Code")
    endif()
    if(DEFINED ENV{RDBASE})
      list(APPEND RDKIT_INCLUDE_SEARCH_PATHS "$ENV{RDBASE}/Code")
    endif()
    if(WIN32)
       list(APPEND RDKIT_INCLUDE_SEARCH_PATHS "C:/RDKit/include" "C:/RDKit/Code")
    else()
       list(APPEND RDKIT_INCLUDE_SEARCH_PATHS
            "/usr/include/rdkit"
            "/usr/local/include/rdkit"
            "/usr/local/rdkit/include/Code"
            "/usr/local/rdkit/include"
            "/usr/local/rdkit/Code"
            "$ENV{HOME}/rdkit/Code" # Use $ENV{HOME} for POSIX compatibility
            "/opt/local/include/rdkit" # MacPorts
            "/opt/homebrew/include/rdkit" # Homebrew ARM
            "/usr/local/opt/rdkit/include" # Homebrew Intel
       )
    endif()

    find_path(RDKIT_INCLUDE_DIR_FOUND # Use a temporary variable
              NAMES GraphMol/RDKitBase.h
              PATHS ${RDKIT_INCLUDE_SEARCH_PATHS}
              # Search system paths last
              PATH_SUFFIXES rdkit Code include/rdkit include/Code include
             )

    if(RDKIT_INCLUDE_DIR_FOUND)
       set(RDKIT_INCLUDE_DIRS ${RDKIT_INCLUDE_DIR_FOUND}) # Set the final variable
       message(STATUS "Found RDKit include files at: ${RDKIT_INCLUDE_DIRS}")
    else()
       message(STATUS "Could not find RDKit include directory. Considered paths: ${RDKIT_INCLUDE_SEARCH_PATHS}")
    endif()
    mark_as_advanced(RDKIT_INCLUDE_DIR_FOUND) # Mark temporary variable advanced
  endif()

  # We need the include directory to be found first
  if(RDKIT_INCLUDE_DIRS AND NOT RDKIT_LIBRARIES)

    # Construct library search paths
    set(RDKIT_LIB_SEARCH_PATHS)
    if(DEFINED ENV{RDKIT_LIB_DIR})
        list(APPEND RDKIT_LIB_SEARCH_PATHS "$ENV{RDKIT_LIB_DIR}")
    endif()
    if(DEFINED ENV{RDKIT_LIB_PATH})
        list(APPEND RDKIT_LIB_SEARCH_PATHS "$ENV{RDKIT_LIB_PATH}")
    endif()
    if(DEFINED ENV{RDKIT_LIBRARIES}) # This env var usually points to a dir
        list(APPEND RDKIT_LIB_SEARCH_PATHS "$ENV{RDKIT_LIBRARIES}")
    endif()
    if(DEFINED ENV{RDKIT_BASE})
        list(APPEND RDKIT_LIB_SEARCH_PATHS "$ENV{RDKIT_BASE}/lib")
    endif()
    if(DEFINED ENV{RDBASE})
        list(APPEND RDKIT_LIB_SEARCH_PATHS "$ENV{RDBASE}/lib")
    endif()
    if(WIN32)
        list(APPEND RDKIT_LIB_SEARCH_PATHS "C:/RDKit/lib")
    else()
        list(APPEND RDKIT_LIB_SEARCH_PATHS
            "/usr/lib"
            "/usr/lib64"
            "/usr/local/lib"
            "/usr/local/lib64"
            "/usr/local/rdkit/lib"
            "$ENV{HOME}/rdkit/lib"
            "/opt/local/lib" # MacPorts
            "/opt/homebrew/lib" # Homebrew ARM/Intel
            "/usr/local/opt/rdkit/lib" # Older Homebrew Intel link
        )
    endif()
    if(DEFINED ENV{LD_LIBRARY_PATH}) # Add LD_LIBRARY_PATH components (Linux/Unix)
        string(REPLACE ":" ";" _ld_path_list "$ENV{LD_LIBRARY_PATH}")
        list(APPEND RDKIT_LIB_SEARCH_PATHS ${_ld_path_list})
    endif()
     if(APPLE AND DEFINED ENV{DYLD_LIBRARY_PATH}) # Add DYLD_LIBRARY_PATH components (macOS)
        string(REPLACE ":" ";" _dyld_path_list "$ENV{DYLD_LIBRARY_PATH}")
        list(APPEND RDKIT_LIB_SEARCH_PATHS ${_dyld_path_list})
    endif()

    # Find a core library first to determine the library directory
    find_library(RDGENERAL_LIB_FOUND # Use temp variable
                 NAMES RDKitRDGeneral RDGeneral
                 HINTS ${RDKIT_LIB_SEARCH_PATHS}
                 )

    if(RDGENERAL_LIB_FOUND)
      GET_FILENAME_COMPONENT(RDKIT_LIBRARY_DIR ${RDGENERAL_LIB_FOUND} PATH)
      message(STATUS "Found RDKit library directory: ${RDKIT_LIBRARY_DIR}")
      # Add the found directory with high priority for subsequent searches
      list(INSERT RDKIT_LIB_SEARCH_PATHS 0 ${RDKIT_LIBRARY_DIR})
      list(REMOVE_DUPLICATES RDKIT_LIB_SEARCH_PATHS) # Clean up

      set(RDKIT_COMPONENT_LIST
          FileParsers
          SmilesParse
          MolTransforms
          Descriptors
          Fingerprints
          MolChemicalFeatures
          ChemicalFeatures
          ChemTransforms
          ChemReactions
          SubstructMatch
          FMCS
          MolAlign
          O3AAlign
          ShapeHelpers
          Depictor
          MolDraw2D
          ForceField
          ForceFieldHelpers
          MolStandardize
          GraphMol
          Subgraphs
          DataStructs
          Catalogs
          FilterCatalog
          ScaffoldNetwork
          RGroupDecomposition
          RDGeometryLib
          RDGeneral
          # Others from your list (add if needed)
          # Abbreviations; Alignment; AvalonLib; CIPLabeler; Deprotect;
          # DistGeomHelpers; DistGeometry; EHTLib; EigenSolvers; FreeSASALib;
          # GeneralizedSubstruct; GenericGroups; InfoTheory; MMPA; MarvinParser;
          # MolCatalog; MolEnumerator; MolHash; MolInteractionFields; MolInterchange;
          # MolProcessing; Optimizer; PartialCharges; PubChemShape; RDBoost;
          # RDInchiLib; RDStreams; RascalMCES; ReducedGraphs; RingDecomposerLib;
          # SimDivPickers; SubstructLibrary; SynthonSpaceSearch; TautomerQuery; Trajectory;
          # avalon_clib; freesasa_clib; ga; hc; pubchem_align3d
          )

      set(RDKIT_LIBRARIES_FOUND) # Initialize empty list for found library paths
      foreach(COMPONENT ${RDKIT_COMPONENT_LIST})
          # Construct names (e.g., RDKitGraphMol, GraphMol)
          string(TOUPPER ${COMPONENT} COMPONENT_UPPER)
          find_library(${COMPONENT_UPPER}_LIB # Variable name, e.g., GRAPHMOL_LIB
                       NAMES RDKit${COMPONENT} ${COMPONENT} # Search names
                       HINTS ${RDKIT_LIB_SEARCH_PATHS} # Use the refined search path list
                       NO_DEFAULT_PATH # Avoid searching standard system paths again if HINTS cover them
                       )
          # If not found without default path, try again *with* default paths
          if(NOT ${COMPONENT_UPPER}_LIB)
               find_library(${COMPONENT_UPPER}_LIB
                       NAMES RDKit${COMPONENT} ${COMPONENT}
                       HINTS ${RDKIT_LIB_SEARCH_PATHS}
                       # Implicitly uses default paths now
                       )
          endif()

          if(${COMPONENT_UPPER}_LIB)
              message(VERBOSE "Found RDKit component ${COMPONENT}: ${${COMPONENT_UPPER}_LIB}")
              list(APPEND RDKIT_LIBRARIES_FOUND ${${COMPONENT_UPPER}_LIB})
              mark_as_advanced(${COMPONENT_UPPER}_LIB) # Mark the individual lib variable
          else()
              message(STATUS "Warning: RDKit component library ${COMPONENT} not found. Searched names: RDKit${COMPONENT}, ${COMPONENT}. Searched paths: ${RDKIT_LIB_SEARCH_PATHS}")
          endif()
      endforeach()

      # Remove duplicates (e.g., if MolChemicalFeatures and ChemicalFeatures point to the same file)
      if(RDKIT_LIBRARIES_FOUND)
          list(REMOVE_DUPLICATES RDKIT_LIBRARIES_FOUND)
          set(RDKIT_LIBRARIES ${RDKIT_LIBRARIES_FOUND} CACHE STRING "List of RDKit libraries found" FORCE)
          message(STATUS "Found RDKit libraries: ${RDKIT_LIBRARIES}")
      endif()

    else()
      message(STATUS "Could not find core RDKit library (RDGeneral). Searched paths: ${RDKIT_LIB_SEARCH_PATHS}")
    endif()
    mark_as_advanced(RDGENERAL_LIB_FOUND RDKIT_LIBRARY_DIR)
  endif()


  find_package_handle_standard_args(RDKit
                                  REQUIRED_VARS RDKIT_LIBRARIES RDKIT_INCLUDE_DIRS
                                  # VERSION_VAR RDKIT_VERSION # Add if you can extract version
                                  )

  # Set RDKIT_INCLUDE_DIR for backward compatibility if needed, but prefer DIRS
  if(RDKIT_INCLUDE_DIRS AND NOT RDKIT_INCLUDE_DIR)
    list(GET RDKIT_INCLUDE_DIRS 0 RDKIT_INCLUDE_DIR)
    set(RDKIT_INCLUDE_DIR ${RDKIT_INCLUDE_DIR} CACHE PATH "First RDKit include directory found")
  endif()
  mark_as_advanced(RDKIT_INCLUDE_DIR RDKIT_LIBRARIES RDKIT_INCLUDE_DIRS RDKIT_LIBRARY_DIR)

endif()

# This makes usage in the main CMakeLists.txt much cleaner
if(RDKIT_FOUND AND NOT TARGET RDKit::RDGeneral) # Check if targets already created
    # Add INTERFACE targets for headers-only aspects
    add_library(RDKit::includes INTERFACE IMPORTED)
    set_target_properties(RDKit::includes PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${RDKIT_INCLUDE_DIRS}"
    )

    # This assumes a shared library build of RDKit, adjust if static
    foreach(LIB_PATH ${RDKIT_LIBRARIES})
        get_filename_component(LIB_NAME ${LIB_PATH} NAME_WE) # Name without lib prefix or extension
        # Try to match LIB_NAME back to a component (this is imperfect)
        string(REGEX REPLACE "^lib" "" COMPONENT_GUESS ${LIB_NAME}) # Remove lib prefix
        string(REPLACE "RDKit" "" COMPONENT_GUESS ${COMPONENT_GUESS}) # Remove RDKit prefix
        if (COMPONENT_GUESS)
           if (NOT TARGET RDKit::${COMPONENT_GUESS})
                add_library(RDKit::${COMPONENT_GUESS} UNKNOWN IMPORTED)
                set_target_properties(RDKit::${COMPONENT_GUESS} PROPERTIES
                    IMPORTED_LOCATION "${LIB_PATH}"
                    INTERFACE_INCLUDE_DIRECTORIES "${RDKIT_INCLUDE_DIRS}" # Simplistic: link headers
                    # Add INTERFACE_LINK_LIBRARIES for dependencies if known
                )
                # Basic dependency guess (most things depend on RDGeneral, GraphMol depends on RDGeneral etc.)
                if (NOT COMPONENT_GUESS STREQUAL "RDGeneral")
                    set_property(TARGET RDKit::${COMPONENT_GUESS} APPEND PROPERTY INTERFACE_LINK_LIBRARIES RDKit::RDGeneral)
                endif()
                if (COMPONENT_GUESS STREQUAL "GraphMol")
                     # GraphMol might depend on DataStructs too, example:
                     # set_property(TARGET RDKit::GraphMol APPEND PROPERTY INTERFACE_LINK_LIBRARIES RDKit::DataStructs)
                endif()
                # Add more specific dependencies here if you know them!
                # Example: RDKit::MolChemicalFeatures might depend on RDKit::GraphMol
                 if (TARGET RDKit::GraphMol AND (COMPONENT_GUESS STREQUAL "MolChemicalFeatures" OR COMPONENT_GUESS STREQUAL "ChemicalFeatures"))
                      set_property(TARGET RDKit::${COMPONENT_GUESS} APPEND PROPERTY INTERFACE_LINK_LIBRARIES RDKit::GraphMol)
                 endif()

           else()
              message(VERBOSE "Target RDKit::${COMPONENT_GUESS} already exists, skipping creation from ${LIB_PATH}")
           endif()
        else()
             message(WARNING "Could not determine RDKit component name from library path: ${LIB_PATH}")
        endif()
    endforeach()

    # Add a convenience target linking common components
    # Adjust this list based on your typical needs
    if (TARGET RDKit::GraphMol AND TARGET RDKit::FileParsers AND TARGET RDKit::SmilesParse AND TARGET RDKit::MolChemicalFeatures)
        add_library(RDKit::rdkit ALIAS RDKit::GraphMol) # Start with a base alias
        # Example: Add common dependencies to an interface library
        add_library(RDKit::common INTERFACE IMPORTED)
        target_link_libraries(RDKit::common INTERFACE
             RDKit::GraphMol
             RDKit::FileParsers
             RDKit::SmilesParse
             RDKit::MolChemicalFeatures # Add the features lib here too
             RDKit::Descriptors
             RDKit::Fingerprints
             # Add others as needed
             RDKit::RDGeneral # Ensure base is linked
        )
    endif()
endif()


function compileInterpolationMat()
    SOURCES = ' InterpolationMex.cpp ';
    SOURCES = [SOURCES 'Interpolateur.cpp '];
    SOURCES = [SOURCES 'InterpolateurPolygoneThiessen.cpp '];
    SOURCES = [SOURCES 'InterpolateurPonderationStations.cpp '];
    SOURCES = [SOURCES 'Station.cpp '];
    SOURCES = [SOURCES 'ListeStation.cpp '];
    SOURCES = [SOURCES 'ParametresInterpolation.cpp '];
    SOURCES = [SOURCES 'Parametres.cpp '];
    SOURCES = [SOURCES 'BassinVersant.cpp '];
    SOURCES = [SOURCES 'Barrage.cpp '];
    SOURCES = [SOURCES 'CarreauEntier.cpp '];
    SOURCES = [SOURCES 'CarreauPartiel.cpp '];
    SOURCES = [SOURCES 'DonneesMeteo.cpp '];
    SOURCES = [SOURCES 'Meteo.cpp '];
    SOURCES = [SOURCES 'DateChrono.cpp '];
    SOURCES = [SOURCES 'Puits.cpp '];
    
    compiler = string(mex.getCompilerConfigurations('c++').Manufacturer);
    CXXFLAGS=" CXXFLAGS=$CXXFLAGS ";
    COMPFLAGS=" COMPFLAGS='$COMPFLAGS ";
    
    if strcmp(compiler, 'Microsoft')
        COMPILER_CXX_FLAGS = "";
        COMPILER_COMP_FLAGS = " /std:c++14 ";
    elseif strcmp(compiler, 'GNU')
        COMPILER_CXX_FLAGS = " -std=c++14 ";
        COMPILER_COMP_FLAGS = "";
    end

    % --- enable/disable logging ---
    log = false;
    LOG_FLAG="";
    if ~log
        LOG_FLAG=" /DSANS_LOG ";
    end
    
    % --- enable/disable debugging ---
    debug = false;
    if debug
        DBG_FLAG = "-g ";
        dbg = "_DBG ";
    else
        DBG_FLAG = " ";
        dbg = " ";
    end
    
    CXXFLAGS=strcat(CXXFLAGS,COMPILER_CXX_FLAGS);
    COMPFLAGS=strcat(COMPFLAGS,COMPILER_COMP_FLAGS,LOG_FLAG,"' ");
    
    OUTFILE = "../mex/cequeauInterpolationMat";
    OUTFILE = strcat(OUTFILE, dbg);

    command = strcat("mex -v -largeArrayDims -output ", OUTFILE, DBG_FLAG, CXXFLAGS, COMPFLAGS, SOURCES);

    eval(command);
end
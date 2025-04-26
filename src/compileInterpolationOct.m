function compileInterpolationOct()

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

    # --- set c++ version ---
    CXXFLAGS=" -std=c++14 ";

    # --- enable/disable logging ---
    log = false;
    COMPFLAGS=" ";

    if ~log
        COMPFLAGS=" -DSANS_LOG ";
    end

    # --- enable/disable debugging ---
    debug = false;
    if debug
        DBG_FLAG = " -g ";
        dbg = "_DBG ";
        FLAGS = " -DENV_OCTAVE ";
    else
        DBG_FLAG = " ";
        dbg = " ";
        FLAGS = " -O2 -DENV_OCTAVE ";
    end

    OUTFILE = " ../mex/cequeauInterpolationOct";
    OUTFILE = strcat(OUTFILE, dbg);
    OUTFILE = char(OUTFILE);
    FLAGS = " -O2 -DENV_OCTAVE ";

    # command = ['mkoctfile -v --mex -O2 -DENV_OCTAVE ' CXXFLAGS DBG_FLAG COMPFLAGS '-o ../mex/cequeauInterpolationOct ' SOURCES];
    command = strcat("mkoctfile --mex ", FLAGS, CXXFLAGS, DBG_FLAG, COMPFLAGS, " -o ", OUTFILE, " ", SOURCES);

    disp(command);
    eval(command);
end

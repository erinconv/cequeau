function paramconv = conversionParametres(paramIn)
    %
    % Conversions de parametres Cequeau 2.x vers Cequeau 4.x
    % 
    % Retraits:
    %  -- parametres.option.calculFonteExterne
    %  
    % Deplacements:
    %  -- parametres.neige -> fonte.cequeau
    %  -- parametres.option.jonei -> fonte.cequeau.jonei
    %  -- parametres.option.joeva -> evapo.cequeau.joeva
    %  
    %  -- parametres.solInitial.tmur -> fonte.cequeau.tmur
    %  -- parametres.solInitial.tstock -> fonte.cequeau.tstock
    %  
    %  -- parametres.sol.evnap -> evapo.cequeau.evnap
    %  -- parametres.sol.xaa -> evapo.cequeau.xaa
    %  -- parametres.sol.xit -> evapo.cequeau.xit
    % 
    %  Ajouts:
    %  -- parametres.option.moduleFonte
    %  -- parametres.option.moduleEvapo
    %  -- parametre.option.calculQualite 

    try 
        parametresWork = paramIn;

        parametresWork.fonte.cequeau       = paramIn.neige;
        parametresWork.fonte.cequeau.jonei = paramIn.option.jonei;
        parametresWork.evapo.cequeau.joeva = paramIn.option.joeva;

        parametresWork.fonte.cequeau.tmur   = paramIn.solInitial.tmur;
        parametresWork.fonte.cequeau.tstock = paramIn.solInitial.tstock;

        parametresWork.evapo.cequeau.evnap = paramIn.sol.evnap;
        parametresWork.evapo.cequeau.xaa   = paramIn.sol.xaa;
        parametresWork.evapo.cequeau.xit   = paramIn.sol.xit;

        parametresWork = rmfield(paramIn, 'neige');
        parametresWork.option = rmfield(paramIn.option, {'calculFonteExterne', 'jonei', 'joeva'});
        parametresWork.solInitial = rmfield(paramIn.solInitial, {'tmur', 'tstock'});
        parametresWork.sol = rmfield(paramIn.sol, {'evnap', 'xaa', 'xit'});

        parametresWork.option.moduleFonte = 1; % 1 = Cequeau
        parametresWork.option.moduleEvapo = 1; % 1 = Cequeau
        parametresWork.option.calculQualite = 0; % 1 = Cequeau

        paramconv = parametresWork;
    catch err
       disp(getReport(err, 'extended'));
       disp('CONVERSION INCOMPLETE');
    end
end





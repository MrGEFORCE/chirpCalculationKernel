function [cph] = load_cfg(cph, dir)
    loadFile = fopen(dir, "r");
    while ~feof(loadFile)
        line = fgetl(loadFile);
        if isempty(line) || line(1) == '#'
            continue;
        end
        
        tokens = regexp(line, '^\s*(\w+)\s*=\s*(.*)\s*$', 'tokens');
        
        if ~isempty(tokens)
            key = tokens{1}{1};
            value = strtrim(tokens{1}{2});
            numValue = str2double(value);
            if value == "yes" || value == "no"
                if value == "yes"
                    cph.cp.(key) = true;
                else
                    cph.cp.(key) = false;
                end
            else
                cph.cp.(key) = numValue;
            end
            
        end
    end
    fclose(loadFile);
    cph = compute_and_validate(cph);
end
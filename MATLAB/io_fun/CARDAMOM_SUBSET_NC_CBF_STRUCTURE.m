function CBFsubset=CARDAMOM_SUBSET_NC_CBF_STRUCTURE(CBF, subset)



CBFsubset=CBF;

f=fields(CBF);

for n=1:numel(f)
    
    if numel(CBFsubset.(f{n}).values)>1
        
    CBFsubset.(f{n}).values=CBFsubset.(f{n}).values(subset);
    end
    
    

    
    
    
end




end






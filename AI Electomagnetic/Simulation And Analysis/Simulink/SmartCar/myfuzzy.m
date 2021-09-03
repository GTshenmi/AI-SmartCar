
%

E = -20.0:1:20.0;
EC = -20.0:1:20.0;
lenE = length(E);
lenEC = length(EC);
U = zeros(lenE,lenEC);

for i = 1 : lenE
    for j = 1 : lenEC
        U(i,j) = sin(E(i) + EC(j));%FuzzyControl(E(i),EC(j));
    end
end

mesh(E,EC,U);

x1 = xlabel('EC');
y1 = ylabel('E');
z1 = zlabel("U");
set(x1,'Rotation',30);
set(y1,'Rotation',30);
set(z1,'Rotation',30);

%plot3(E,EC,reshape(U',1,len * len))



function Result = FuzzyControl(e,ec)

%     SEFF = [-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0];                     
% 
%     SDFF = [-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0];
%     
%     %SDFF = [-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0];
%     
%     SUFF = [-1.0,-0.8,-0.6,-0.4,-0.2,-0.1,0.0,0.1,0.2,0.4,0.6,0.8,1.0];
% 
%     SFuzzyRule =[-6,-6,-6,-6,-5,-5,-4;
%                 -5,-5,-5,-4,-4,-3,-1;
%                 -4,-4,-3,-1, 0, 1, 2;
%                 -4,-3,-1, 0, 1, 3, 4;
%                 -2,-1, 0, 1, 3, 4, 4;
%                 1, 3, 4, 4, 5, 5, 5;
%                 4, 5, 5, 6, 6, 6, 6];


    SEFF = [-80.0,-40.0,-20.0,0.0,20.0,40.0,80.0];                     

    SDFF = [-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0];
    
    %SDFF = [-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0];
    
    SUFF = [-0.8,-0.6,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.6,0.8];
                %--------EC---------
    SFuzzyRule =[-6,-6,-6,-6,-5,-4,-3;%e
                -6,-5,-4,-3,-3,-2,-1;% |
                -5,-4,-3,-1, 0, 1, 2;% |
                -4,-3,-1, 0, 1, 3, 4;
                -2,-1, 0, 1, 3, 4, 5;
                1, 2, 3, 3, 4, 5, 6;
                3, 4, 5, 6, 6, 6, 6];
    
    EF = zeros(1,2);
    DF = zeros(1,2);

    En = 0;
    Dn = 0;

    %Un = zeros(1,4);
    Un = [0,0,0,0];

    UF = zeros(1,4);

    U = zeros(1,13);

    for i = 1:13
        U(i) = 0;
    end

    if e < SEFF(1)

        EF(1) = 1.0;
        EF(2) = 0.0;

        En = -2;

    elseif e >= SEFF(7)
    
        EF(1) = 0.0;
        EF(2) = 1.0;
        En = 3;
    
    else 
        for i = 2 : 7
            if ((e < SEFF(i)) && (e >= SEFF(i - 1)))
            
                En = i - 1 - 3;

                EF(1) = -(e - SEFF(i)) / (SEFF(i) - SEFF(i - 1));

                EF(2) = (e - SEFF(i - 1)) / (SEFF(i) - SEFF(i - 1));
                
            end
        end
    end

    if (ec < SDFF(1))
    
        DF(1) = 1.0;
        DF(2) = 0.0;

        Dn = -2;
    
    elseif (ec >= SDFF(7))
    
        DF(1) = 0.0;
        DF(2) = 1.0;
        Dn = 3;
    
    else
    
        for i = 2 : 7 
        
            if (ec < SDFF(i) && ec >= SDFF(i - 1)) 
            
                Dn = i - 1 - 3;

                DF(1) = -(ec - SDFF(i)) / (SDFF(i) - SDFF(i - 1));

                DF(2) = (ec - SDFF(i - 1)) / (SDFF(i) - SDFF(i - 1));
            
            end
        end
    end

    Un(1) = SFuzzyRule(En + 3,Dn + 3) + 6 + 1;     

    UF(1) = min(EF(1), DF(1));

    Un(2) = SFuzzyRule(En + 3 + 1,Dn + 3) + 6 + 1;
    UF(2) = min(EF(2), DF(1));

    Un(3) = SFuzzyRule(En + 3,Dn + 3 + 1) + 6 + 1;
    UF(3) = min(EF(1), DF(2));

    Un(4) = SFuzzyRule(En + 3 + 1,Dn + 3 + 1) + 6 + 1;
    UF(4) = min(EF(2), DF(2));
    
    if (Un(1) == Un(2))    
        if (UF(1) > UF(2))
            UF(2) = 0;
        else
            UF(1) = 0;
        end
    end
    if (Un(1) == Un(3))
    
        if (UF(1) > UF(3))
            UF(3) = 0;
        else
            UF(1) = 0;
        end
        
    end
    
    if (Un(1) == Un(4))
    
        if (UF(1) > UF(4))
            UF(4) = 0;
        else
            UF(1) = 0;
        end
    end

    if (Un(2) == Un(3))
    
        if (UF(2) > UF(3))
            UF(3) = 0;
        else
            UF(2) = 0;
        end
        
    end
    
    if (Un(2) == Un(4))
    
        if (UF(2) > UF(4))
            UF(4) = 0;
        else
            UF(2) = 0;
        end
        
    end
    

    if (Un(3) == Un(4))
    
        if (UF(3) > UF(4))
            UF(4) = 0;
        else
            UF(3) = 0;
        end
        
    end
    

    for i = 1 : 4
    
        U(Un(i)) = U(Un(i)) + UF(i);
        
    end

    sum0 = 0.0;
    sum1 = 0.0;

    for i = 1 : 13
        sum0 = sum0 + U(i) * SUFF(i);
        sum1 = sum1 + U(i);
    end

    Result = sum0 / sum1;
    
end
% 
% float FuzzyControl(fuzzy_ctrl_t *fuzzy, float target, float actual)
% {
%     float EF[2] = { 0.0,0.0 }, DF[2] = {0.0,0.0};
% 
%     int i = 0;
% 
%     int En = 0, Dn = 0;
% 
%     sint32_t Un[4];
% 
%     float UF[4];
% 
%     float U[13];
% 
%     for (i = 0; i < 13; i++)
%     {
%         U[i] = 0.0;
%     }
% 
%     /*Calculate E and Ec.*/
% 
%     fuzzy->e[0] = fuzzy->e[1];
%     fuzzy->e[1] = target - actual;
% 
%     fuzzy->ec = fuzzy->e[1] - fuzzy->e[0];
% 
%     /*Calculate Membership.*/
% 
%     if (fuzzy->e[1] < SEFF[0])
%     {
%         EF[0] = 1.0;
%         EF[1] = 0.0;
% 
%         En = -3;
%     }
%     else if (fuzzy->e[1] >= SEFF[6])
%     {
%         EF[0] = 0.0;
%         EF[1] = 1.0;
%         //En = 3;
%         En = 2;
%     }
%     else
%     {
%         for (i = 1; i <= 6; i++)
%         {
%             if ((fuzzy->e[1] < SEFF[i]) && (fuzzy->e[1] >= SEFF[i - 1])) //??2?????¨°?¨²?¡ì
%             {
%                 En = i - 1 - 3;
% 
%                 EF[0] = -(fuzzy->e[1] - SEFF[i]) / (SEFF[i] - SEFF[i - 1]);
% 
%                 EF[1] = (fuzzy->e[1] - SEFF[i - 1]) / (SEFF[i] - SEFF[i - 1]);
%             }
%         }
%     }
% 
%     if (fuzzy->ec < SDFF[0])
%     {
%         DF[0] = 1.0;
%         DF[1] = 0.0;
% 
%         Dn = -3;
%     }
%     else if (fuzzy->ec >= SDFF[6])
%     {
%         DF[0] = 0.0;
%         DF[1] = 1.0;
%         //Dn = 3;
%         Dn = 2;
%     }
%     else
%     {
%         for (i = 1; i <= 6; i++)
%         {
%             if (fuzzy->ec < SDFF[i] && fuzzy->ec >= SDFF[i - 1]) //??2?????¨°?¨²?¡ì
%             {
%                 Dn = i - 1 - 3;
% 
%                 DF[0] = -(fuzzy->ec - SDFF[i]) / (SDFF[i] - SDFF[i - 1]);
% 
%                 DF[1] = (fuzzy->ec - SDFF[i - 1]) / (SDFF[i] - SDFF[i - 1]);
%             }
%         }
%     }
% 
%     /*Fuzzy Reasoning.*/
% 
%     Un[0] = SFuzzyRule[En + 3][Dn + 3] + 6;      //2*2 = 4?????¨°?¨²?¡ì
% 
%     UF[0] = min(EF[0], DF[0]);
% 
%     Un[1] = SFuzzyRule[En + 3 + 1][Dn + 3] + 6;
%     UF[1] = min(EF[1], DF[0]);
% 
%     Un[2] = SFuzzyRule[En + 3][Dn + 3 + 1] + 6;
%     UF[2] = 1.0 * min(EF[0], DF[1]);
% 
%     Un[3] = SFuzzyRule[En + 3 + 1][Dn + 3 + 1] + 6;
%     UF[3] = 1.0 * min(EF[1], DF[1]);
% 
%     if (Un[0] == Un[1])    //???????????????¨®
%     {
%         if (UF[0] > UF[1])
%             UF[1] = 0;
%         else
%             UF[0] = 0;
%     }
%     if (Un[0] == Un[2])
%     {
%         if (UF[0] > UF[2])
%             UF[2] = 0;
%         else
%             UF[0] = 0;
%     }
%     if (Un[0] == Un[3])
%     {
%         if (UF[0] > UF[3])
%             UF[3] = 0;
%         else
%             UF[0] = 0;
%     }
% 
%     if (Un[1] == Un[2])
%     {
%         if (UF[1] > UF[2])
%             UF[2] = 0;
%         else
%             UF[1] = 0;
%     }
%     if (Un[1] == Un[3])
%     {
%         if (UF[1] > UF[3])
%             UF[3] = 0;
%         else
%             UF[1] = 0;
%     }
% 
%     if (Un[2] == Un[3])
%     {
%         if (UF[2] > UF[3])
%             UF[3] = 0;
%         else
%             UF[2] = 0;
%     }
% 
%     for (i = 0; i < 4; i++)
%     {
%         U[Un[i]] += UF[i];
%     }
% 
%     /*Anti Fuzzy(Gravity Method)*/
% 
%     float sum0 = 0.0;
%     float sum1 = 0.0;
% 
%     for (i = 0; i < 13; i++)
%     {
%         sum0 += U[i] * SUFF[i];
% 
%         sum1 += U[i];
%     }
% 
%     if (fabs(sum1) < 1e-6)
%         fuzzy->U = 0.0;
%     else
%         fuzzy->U = sum0 / sum1;
% 
%     return fuzzy->U;
% }
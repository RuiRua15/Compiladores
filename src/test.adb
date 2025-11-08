procedure Main is
   a, b, counter : Integer;
begin
   a := 100 + 2 * 3 - 10 / 5; 
   b := a rem 10;                
   counter := 0;
   
   if a >= 100 then  
      if b = 4 then  
         Put_Line(1);
      else
         Put_Line(99);
      end if;
   end if;

   while counter < 5 loop
      
      if counter <= 2 then  
         a := a - 1;
      else                  
         if a /= 0 then  
            a := a + 1;
         end if;
      end if;

      counter := counter + 1;
      
   end loop;

   Put_Line(a + Get_Line); 

end Main;

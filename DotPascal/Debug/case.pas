program procTest;    
  var i,j:integer;
  begin 
   case i of
    0:      
      begin
      Write(' outer 0\n');
       j := i + 1;
       case j of
         0:
          Write( ' inner 0\n');
         1:
          Write( ' inner 1\n');         
         default:
	   Write('inner default');           
       end
      end 
    4:
      Write(' outer 4\n');
    default:
      Write(' outer default');       
    
    end
  end.

program procTest;    
  var i:integer;
  function ReadInt():integer;
  var j:integer;
  begin
   ReadInt := Read();
   ReadInt :=  ReadInt - 48;
  end  
  
  begin 
   i := ReadInt();
   Write(i);   
  end.

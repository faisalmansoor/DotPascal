program procTest; 
   var i:integer;         
  procedure proc( ub :integer );
  var j:integer;
      a:array[1..10] of integer;
  begin      
    for i := 10 downto 1 do
         a[i] := i;
    for i := 10 downto 1 do
         Write(a[i],'\n');
  end  
  begin  
    i := 0;     
    proc(10);
  end.

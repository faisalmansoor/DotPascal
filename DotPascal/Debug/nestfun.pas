program procTest;  
  var i:integer;
      array i[1..2,1..2] of integer;	
  
  function sum(a,b:integer):integer;  
  begin
  	sum := a + b;
  end  
  begin
     Write(sum(7,sum(1,sum(2,sum(sum(4,5),4)))));
  end.

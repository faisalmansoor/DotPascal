program procTest;    
  var i:integer;
  begin
    Write('Enter any integer: ');
    i := ReadInt();
     
      if i > 10 then
       Write('You entered ',i,' which is greater than 10');
      else
   Write('You entered ',i,' which is less than 10');  
  end.

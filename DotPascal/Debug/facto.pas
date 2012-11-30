program procTest; 
  var i:integer;  
  function factorial ( N :integer ):integer;
  {
    Computes N!
    Pre : N >= 0
    Post: Returns the product 1*2*3 .... * N for N > 0
          Returns 1 for N = 0
  }
  var i:integer;
  begin   
   if  N <= 1 then
     factorial := 1;   
   else 
     factorial := N * factorial( N - 1);
    
  end  
  begin  
   Write('Enter any number to caluculate its factorial :');
   i := Read();
   i := i - 48; { convert to integer the char }
  Write('Factorial of ',i,' = ',factorial(i));  
  end.

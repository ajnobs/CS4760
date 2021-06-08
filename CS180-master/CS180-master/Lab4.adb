WITH ADA.TEXT_IO; USE ADA.TEXT_Io;
WITH ADA.INTEGER_TEXT_IO; USE ADA.INTEGER_TEXT_IO;
PROCEDURE Lab4 IS


   FUNCTION Is_Even(N : Natural) RETURN Boolean IS
--<>------------------Subprogram Description---------------------<>--
   --This subprogram receives a natural number and returns True
   --if it is an even number and False if it is an odd number.
--<>-------------------------------------------------------------<>--
      Return_Val : Boolean;

   BEGIN -- Is_Even
      IF (N rem 2) = 0 THEN
         --N is even
         Return_Val := True;
      ELSE
         --N is odd
         Return_Val := False;
      END IF;
      RETURN Return_Val;
   END Is_Even;


   PROCEDURE Print_Hailstone_Sequence (N_Fp : IN Natural) IS
--<>-------------------Subprogram Description--------------------<>--
   --This subprogram receives a natural number then computes
   --and outputs its hailstone sequence.
--<>-------------------------------------------------------------<>--
      Local_N : Natural := N_Fp;

   BEGIN --Print_Hailstone_Sequence
      PUT("The hailstone sequence is: ");
      LOOP
         --Print next number in the sequence
         PUT(Item => Local_N, Width => 1);
         EXIT WHEN Local_N = 1;
         PUT(", ");
         IF Is_Even(Local_N) THEN
            --Even calculation
            Local_N := Local_N / 2;
         ELSE
            --Odd calculation
            Local_N := (Local_N * 3) + 1;
         END IF;
      END LOOP;
   END Print_Hailstone_Sequence;


---------------Declaration Section for Main Program------------------
   N : Natural; --Hailstone Number
---------------------------------------------------------------------

BEGIN --Lab4 / BEGIN Main Program
   PUT("Please enter the number whose hailstone sequence you wish to compute:");
   GET(N);
   New_Line(2);
   Print_Hailstone_Sequence(N);
END Lab4; --END Main Program

WITH Ada.Text_IO; USE Ada.Text_IO;
WITH Ada.Integer_Text_IO; USE Ada.Integer_Text_IO;
WITH Ada.Float_Text_IO; USE Ada.Float_Text_IO;
PROCEDURE Grade IS
-----------------------------------------------------
--Name: Aj Nobs
--Class: CS 180-03
--Program: Lab Three
--------------- Program Descriptions ----------------
--This program reads external data files that
--represent the scores and outputs a letter grade
--as well as an appropriate identifying message.
----------------- Identifier Table ------------------
--Num    -
--Score  - The score a student received
-----------------------------------------------------
   Num   : Natural;
   Score : Natural := 0;

BEGIN -- Grade
   WHILE NOT End_Of_Loop
      PUT(Item => "Student ");
      GET(Item => Num);
      PUT(Item => Num + 1)
      PUT(Item => " received ");
      GET(Item => Grade)
      IF Grade >= 90 THEN
      	PUT(Item => "an A.");
      	END IF;
      ELSIF Grade >= 80 THEN
      	PUT(Item => "a B.");
      	END IF;
      ELSIF Grade >= 70 THEN
      	PUT(Item => "a C.");
      	END IF;
      ELSIF Grade >= 60 THEN
      	PUT(Item => "a D.");
      	END IF;
      ELSIF Grade < 60 THEN;
      	PUT(Item => "an F.");
      	END IF;
      END IF;
    END LOOP;
END Grade;

WITH Ada.Text_Io;USE Ada.Text_Io;
WITH Ada.Float_Text_Io;USE Ada.Float_Text_Io;
WITH Ada.Integer_Text_Io;USE Ada.Integer_Text_Io;
PROCEDURE Prog1 IS
-----------------------------------------------------------
--Name:    Aj Nobs
--Class:   CS 180 - 3
--Program: One
--------------- Program Description -----------------------
--This program finds the average speed, after prompting the
--user to input the travel time and distance.
--------------- Identifier Table --------------------------
--How_Far   - Distance traveled
--How_Fast  - Speed of travel
--How_long  - Travel time
--Name      - User's Name
-----------------------------------------------------------
How_Far      : Natural;
How_Fast     : Float;
How_Long     : Float;
Name         : String(1..10);

BEGIN -- Prog1
   --Prompt user for name
   Put(Item =>"Please enter your name using exactly 10 characters.");
   New_Line;
   Put(Item =>"Remember to press ENTER after each item is input");
   New_Line;
   Get(Item =>Name);
   --Prompt user for hours and distance.
   Put(Item =>"Enter the number of hours driven (as a float): ");
   New_Line;
   Get(Item =>How_Long);
   Put(Item =>"Enter the miles traveled (as a Natural): ");
   Get(Item =>How_Far);
   --Compute the averaqge speed
   New_Line;
   How_Fast := Float(How_Far) / How_Long ;
   Put(Item =>"You traveled at an average speed of ");
   Put(Item =>How_Fast, Fore=>1, Aft=>1, Exp=>0);
   Put(Item =>" miles per hour.");
   New_Line;
   Put(Item =>"Thank you for using this program!");
end prog1;

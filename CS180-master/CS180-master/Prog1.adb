WITH Ada.Text_IO; USE Ada.Text_IO;
WITH Ada.Float_Text_IO; USE Ada.Float_Text_IO;
WITH Ada.Integer_Text_IO; USE Ada.Integer_Text_IO;
PROCEDURE Prog1 IS
------------------------------------------------------------
--Name:    Aj Nobs
--Class:   CS 180-03
--Program: One
------------------- Program Description --------------------
--This program takes the items purchased and calculates the
--subtotal, the tax, the discount (if discount coupon is
--used), and the total that accounts for all previously
--mentioned variables.
-------------------- Identifier Table  ---------------------
--Pens        - Number of pens purchased
--Pens_Pr     - Price of pens
--Calc        - Number of calculators purchased
--Calc_Pr     - Price of calculators
--Noteb       - Number of notebooks purchased
--Noteb_Pr    - Price of notebooks
--Disks       - Number of disks purchased
--Disks_Pr    - Price of disks
--Sub_Total   - Net total for the customer
--Disc_Per    - Discount percentage
--Disc        - Dollar amount of discount
--Tax         - Tax for items purchased
--Total       - Total amount due
------------------------------------------------------------
 Pens         : Integer;
 Pens_Pr      : constant Float := 0.75;
 Calc         : Integer;
 Calc_Pr      : constant Float := 25.95;
 Noteb        : Integer;
 Noteb_Pr     : constant Float := 2.75;
 Disks        : Integer;
 Disks_Pr     : constant Float := 1.75;
 Sub_Total    : Float;
 Disc_Per     : Integer;
 Disc         : Float;
 Tax          : Float;
 Total        : Float;

BEGIN -- Prog1
   --Prompt user for purchase info
   Put(Item => "Please enter the number of pens purchased: ");
   Get(Item => Pens);
   Put(Item => "Please enter the number of calculators purchased: ");
   Get(Item => Calc);
   Put(Item => "Please enter the number of notebooks purchased: ");
   Get(Item => Noteb);
   Put(Item => "Please enter the number of disks purchased: ");
   Get(Item => Disks);
   New_Line;
   Put(Item => "Please enter the percent discount: ");
   Get(Item => Disc_Per);
   New_Line;
   New_Line;
   --Compute Sub_Total, Disc, Tax, and Total
   Sub_Total := (Float(Pens) * Pens_Pr) + (Float(Calc) * Calc_Pr) + (Float(Noteb) * Noteb_Pr) + (Float(Disks) * Disks_Pr);
   Put(Item => "Net Total for Customer: $");
   Put(Item => Sub_Total, Fore=> 2, Aft=> 2, Exp=> 0);
   New_Line;
   Disc := (Float(Disc_Per) / 100.0) * Sub_Total;
   Put(Item => "Discount: $");
   Put(Item => Disc, Fore=> 2, Aft=> 2, Exp=> 0);
   New_Line;
   Tax := (Sub_Total - Disc) * 0.07;
   Put(Item => "Tax: $");
   Put(Item => Tax, Fore=> 2, Aft=> 2, Exp=> 0);
   New_Line;
   Total:= Sub_Total - Disc + Tax;
   Put(Item => "Total Amount Due: $");
   Put(Item => Total, Fore=> 2, Aft=> 2, Exp=> 0);
END Prog1;


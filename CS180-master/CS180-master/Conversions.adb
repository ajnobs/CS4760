--WITH Ada.Text_IO; USE Ada.Text_IO;
--WITH Ada.Float_Text_IO; USE Ada.Float_Text_IO;
--PROCEDURE Conversions IS
-----------------------------------------------------------
---- Conversions.adb
----
---- A function to convert Celsius temperatures
---- to fahrenheit temperatures.
----
---- Written by Ima Programmer
----
---- Date:  September 2, 2014
-----------------------------------------------------------
--Celsius, Fahrenheit : Float;
--
--begin -- conversions
--   Put("Please enter a celsius temperature: ");
--   Get(Celsius);
--   Fahrenheit := 9.0 / 5.0 * Celsius + 32.0;
--   New_Line;
--   Put(Item=>Celsius, Fore => 1, Aft => 2, Exp => 0);
--   Put(" degrees Celsius is ");
--   Put(Item => Fahrenheit, Fore => 1, Aft => 5, Exp=> 0);
--   Put(" degrees Fahrenheit.");
--END Conversions;


WITH Ada.Text_IO; USE Ada.Text_IO;
WITH Ada.Float_Text_IO; USE Ada.Float_Text_IO;
PROCEDURE Conversions IS
---------------------------------------------------------
--Name:   Aj Nobs
--Class:  CS 180-03
--Lab:    Two(9/3/14)
---------------------------------------------------------
--This program converts a distance in kilometers
--into a distance in miles.
---------------------------------------------------------
Kilometers : Float;
Miles      : Float;

BEGIN -- conversions
   Put("Please enter a distance in kilometers: ");
   Get(Kilometers);
   Miles := Kilometers * 0.62137;
   New_Line;
   Put(Item=>Kilometers, Fore => 3, Aft => 5, Exp => 0);
   Put(" kilometers is equal to ");
   Put(Item =>Miles, Fore =>3, Aft => 5, Exp => 0);
   Put(" miles.");
END Conversions;

WITH Ada.Text_IO; USE Ada.Text_IO;
WITH Ada.Integer_Text_IO; USE Ada.Integer_Text_IO;
WITH Ada.Float_Text_IO; USE Ada.Float_Text_IO;
PROCEDURE Prog2 IS
---------------------------------------------------
--Name : Aj Nobs
--Class : CS 180-03
--Program : Two
--Program Description------------------------------
--This program will take the data input from
--transactions made and give the resulting credit
--or debt total depending on the type of
--transaction and mutual fund along with its value.
--Identifer Table----------------------------------
--Com         - The number multipied with the value to get the value of the commission
--Com_Per     - The percentage of the transaction value charged as commission
--Com_Val     - The value of the commission
--Fund_Type   - The type of stock/mutual fund (load or no load)
--Num         - Transaction number
--Space       - Space in data file
--Total       - The value of stock including commission if applicable
--Trans_Type  - The type of transaction (Buy or Sell)
--Val         - The value of the stock that was bought or sold in the transaction
--------------------------------------------------
   Com        : Float;
   Com_Per    : Integer;
   Com_Val    : Float;
   Fund_Type  : Character;
   Num        : Natural := 0;
   Space      : Character;
   Total      : Float;
   Trans_Type : Character;
   Val        : Float;

begin --Prog2
  WHILE NOT End_of_File LOOP
    --Transaction Counter
    PUT("Transaction Number:");
    Num := Num + 1;
    SET_COL(23);
    PUT(Num, WIDTH => 1);
    NEW_LINE;
    --Transaction Type & Load Type
    GET(Trans_Type);
    IF Trans_Type = 'B'  THEN
       --Buy / -
       PUT("Transaction Type");
       SET_COL(19);
       PUT(":");
       SET_COL(23);
       PUT("Buy");
       NEW_LINE;
       GET(Val);
       GET(Space);
       GET(Fund_Type);
       IF Fund_Type = 'N' THEN
          --Buy / N
          Total := Val;
          PUT("Debit Amount");
          SET_COL(19);
          PUT(":");
          SET_COL(23);
          PUT("$");
          PUT(Total, 1, 2, 0);
          NEW_LINE;
          NEW_LINE;
       ELSIF Fund_Type = 'L' THEN
          --Buy / L
          GET(Fund_Type);
          GET(Com_Per);
          Com := Float(Com_Per) / 100.00;
          Com_Val := Com * Val;
          Total := Val + Com_Val;
          PUT("Debit Amount");
          SET_COL(19);
          PUT(":");
          SET_COL(23);
          PUT("$");
          PUT(Total, 1, 2, 0);
          NEW_LINE;
          NEW_LINE;
       ELSE
          PUT("An error has occured in processing the transaction information.  Please check input file and ensure the format is correct.");
       END IF;
    ELSIF Trans_Type = 'S' THEN
       --Sell / -
       PUT("Transaction Type");
       SET_COL(19);
       PUT(":");
       SET_COL(23);
       PUT("Sell");
       NEW_LINE;
       GET(Val);
       GET(Space);
       GET(Fund_Type);
       IF Fund_Type = 'N' THEN
          --Sell / N
          Total := Val;
          PUT("Credit Amount");
          SET_COL(19);
          PUT(":");
          SET_COL(23);
          PUT("$");
          PUT(Total, 1, 2, 0);
          NEW_LINE;
          NEW_LINE;
       ELSIF Fund_Type = 'L' THEN
          --Sell / L
          GET(Fund_Type);
          GET(Com_Per);
          Com := Float(Com_Per) / 100.0;
          Com_Val := Com * Val;
          Total := Val - Com_Val;
          PUT("Credit Amount");
          SET_COL(19);
          PUT(":");
          SET_COL(23);
          PUT("$");
          PUT(Total, 1, 2, 0);
          NEW_LINE;
          NEW_LINE;
       ELSE
          PUT("An error has occured in processing the transaction information.  Please check input file and ensure the format is correct.");
       END IF;
    END IF;
  END LOOP;
END Prog2;

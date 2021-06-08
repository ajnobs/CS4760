WITH Ada.Text_Io; USE Ada.Text_Io;
WITH Ada.Integer_Text_Io; USE Ada.Integer_Text_Io;
WITH Ada.Float_Text_Io; USE Ada.Float_Text_Io;
PROCEDURE lab5 IS

	FUNCTION Compute_Sales_Price (
		Orig_Price_FP        : Float;
		Reduction_Percent_FP : Natural)
		RETURN Float IS
	-----------------------------------Subprogram Description---------------------------------------
	--This subprogram receives an original price and a reduction percentage.
	--It computes and returns the new amount based on the sale.
	------------------------------------------------------------------------------------------------
		Return_Val : Float;
	BEGIN --Compute_Sales_Price
		Return_Val := Orig_Price_FP- Orig_Price_FP * Float(Reduction_Percent_FP)/100.0;
		RETURN Return_Val;
	END Compute_Sales_Price;

	PROCEDURE Print_Headings IS
	-----------------------------------Subprogram Description---------------------------------------
	--This procedure prints the report heading and the column heading/underline.
	------------------------------------------------------------------------------------------------
	BEGIN
		--Print Report Heading
		SET_COL(30);
		PUT("Village Variety Store");
		NEW_LINE(2);
		--Print Column Heading
		PUT("Item Code");
		SET_COL(12);
		PUT("Number Sold");
		SET_COL(25);
		PUT("Original Price");
		SET_COL(42);
		PUT("Reductions");
		SET_COL(57);
		PUT("Sales Price");
		SET_COL(75);
		PUT("Income");
		NEW_LINE;
		--Print Column Underline
		PUT("---------");
		SET_COL(12);
		PUT("-----------");
		SET_COL(25);
		PUT("--------------");
		SET_COL(42);
		PUT("----------");
		SET_COL(57);
		PUT("-----------");
		SET_COL(74);
		PUT("---------");
		NEW_LINE;
	END Print_Headings;

	FUNCTION Compute_Lost_Revenue (
		Orig_Price_FP    :  Float;
		Sales_Price_FP   :  Float;
		Num_Sold_FP      :  Natural;
		Lost_Revenue_FP  :  Float)
		RETURN Float IS
	-----------------------------------Subprogram Description---------------------------------------
	--This function receives the lost revenue thus far, adds the new lost revenue, and returns the 
	--updated amount to the main program.
	------------------------------------------------------------------------------------------------
		Return_Loss : Float;
	BEGIN
		Return_Loss := Lost_Revenue_FP + ((Orig_Price_FP - Sales_Price_FP) * Float(Num_Sold_FP));
		RETURN Return_Loss;
	END Compute_Lost_Revenue;

	FUNCTION Compute_Total_Income (
		Income_FP        :  Float;
		Total_Income_FP  :  Float)
		RETURN Float IS
	-----------------------------------Subprogram Description---------------------------------------
	--This function receives the total income thus far, updates the total with the new income and
	--returns the new updated amount to the main program.
	------------------------------------------------------------------------------------------------
		Return_Income : Float;
	BEGIN
		Return_Income := Total_Income_FP + Income_FP;
		RETURN Return_Income;
	END Compute_Total_Income;

	PROCEDURE Find_Income (
		Sales_Fp    : IN     Float;
		Num_Sold_Fp : IN     Natural;
		Income_Fp   :    OUT Float) IS
	-----------------------------------Subprogram Description---------------------------------------
	--This subprogram receives the sales price and the number sold of an item and
	--returns the income.
	------------------------------------------------------------------------------------------------
	BEGIN
		Income_Fp := Sales_Fp * Float(Num_Sold_Fp);
	END Find_Income;

-------------------------------Declaration Section for Main Program--------------------------------
	Item_Code         :  String (1 .. 2);       --Item Code
	Num_Sold          :  Natural;               --Number of Items Sold
	Original_Price    :  Float;                 --Original Price
	Reduction_Percent :  Natural;               --Percent Reduction
	Sales_Price       :  Float;                 --Sales Price
	Income            :  Float;                 --Income
	Lost_Revenue      :  Float := 0.0;          --Total revenue lost from price reduction
	Total_Income      :  Float := 0.0;          --Total income for the day
	Grand_Total_Sold  :  Natural := 0;          --Counter for each record
---------------------------------------------------------------------------------------------------
BEGIN --Lab5 (Begin of main program)
	Print_Headings;
	NEW_LINE;
	--Chart fill loop
	LOOP
		GET(Item_Code);
		EXIT WHEN Item_Code = "-9";
		GET(Num_Sold);
		--Calculate Grand_Total_Sold
		Grand_Total_Sold := Grand_Total_Sold + Num_Sold;
		GET(Original_Price);
		GET(Reduction_Percent);
		--Calculate Sales_Price
		Sales_Price := Compute_Sales_Price(Original_Price, Reduction_Percent);
		SET_COL(4);
		PUT(Item_Code);
		Set_Col(15);
		PUT(Num_Sold,1);
		SET_COL(30);
		PUT(Original_Price,1,2,0);
		SET_COL(45);
		PUT(Reduction_Percent, 1);
		PUT('%');
		SET_COL(60);
		PUT('$');
		PUT(Sales_Price,1,2,0);
		--Retrieve Income
		Find_Income(Sales_Price, Num_Sold, Income);
		SET_COL(75);
		PUT('$');
		PUT(Income, 1,2,0);
		--Calculate Total Income
		Total_Income := Compute_Total_Income(Income, Total_Income);
		--Calculate Total Revenue Lost
		Lost_Revenue := Compute_Lost_Revenue(Original_Price, Sales_Price, Num_Sold, Lost_Revenue);
		NEW_LINE;
	END LOOP;
	NEW_LINE(3);
	--Print Daily Summary
	PUT("Daily Summary:");
	NEW_LINE;
	PUT("-------------");
	NEW_LINE;
	--Total # Sold
	PUT("Total Number of Items Sold:");
	SET_COL(30);
	PUT(Grand_Total_Sold,1);
	NEW_LINE;
	--Total Revenue Lost
	PUT("Revenue Lost due to Reductions:");
	SET_COL(34);
	PUT("$");
	PUT(Lost_Revenue, 1, 2, 0);
	NEW_LINE;
	--Total Income
	PUT("Total Income:");
	SET_COL(16);
	PUT("$");
	PUT(Total_Income, 1, 2, 0);
END lab5; --End of main program

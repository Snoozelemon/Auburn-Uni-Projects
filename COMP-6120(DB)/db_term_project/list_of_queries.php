<!DOCTYPE html>
<html>
	<head>
		<link rel="icon" href="./img/website-icon.png" type="image/png">
		<link rel="preconnect" href="https://fonts.googleapis.com">
	    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<style>
			
			/* width of scroll bar */
			::-webkit-scrollbar {
			  width: 6px;
			}
			/* Track */
			::-webkit-scrollbar-track {
			  background: #f1f1f1; 
			}
			 
			/* Handle */
			::-webkit-scrollbar-thumb {
			  background: #888; 
			}

			/* Handle on hover */
			::-webkit-scrollbar-thumb:hover {
			  background: #555; 
			}

			html{
				background-color: black;
				color: whitesmoke;
				font-family: 'Archivo Black', sans-serif;
		      	font-family: 'EB Garamond', serif;
		      	font-family: 'Nunito', sans-serif;
			}
			.collapsible {
			  	background-color: #777;
			  	color: white;
			  	cursor: pointer;
			  	padding: 18px;
			  	width: 100%;
				border: none;
				text-align: left;
				outline: none;
				font-size: 15px;
			}
				.active, .collapsible:hover {
				  background-color: #555;
				}

				.content {
				  padding: 0 18px;
				  max-height: 0;
				  overflow: hidden;
				  transition: max-height 0.2s ease-out;
				  background-color: transparent;
				}
				h2{
					width: 500px;
					float: left;
				}
				#return{
					margin: 20px;
					float: right;
					font-size: 20px;
					background-color: transparent;
					color: whitesmoke;
					border: 2px solid whitesmoke;
					border-radius: 5px;
					cursor: pointer;
				}
				#return:hover{
					background-color: whitesmoke;
					color: black;
				}
		</style>
	</head>
	<body>
		<div>
			<h2>Some SQL Queries to test on the website.</h2>
			<button id="return" onClick="window.location.href='https://webhome.auburn.edu/~szc0239/query_page.php';">SQL Editor</button>
		</div>
		

		<button class="collapsible">1. Show the subject names of books supplied by *supplier2*.</button>
		<div class="content">
		  <p>Select s.CategoryName as SubNames from db_book b, db_subject s, db_supplier sup
			where  b.SupplierID = sup.SupplierID AND b.SubjectID = s.SubjectID AND sup.CompanyName = 'supplier2';</p>
		</div>
		<button class="collapsible">2. Show the name and price of the most expensive book supplied by "supplier3"
</button>
		<div class="content">
		  <p>SELECT B.Title, B.UnitPrice
	FROM db_book B, db_supplier Supp
	WHERE B.SupplierID = Supp.SupplierID AND B.UnitPrice = (select MAX(B2.UnitPrice)
		From db_book B2, db_supplier Supp2
		Where B2.SupplierID = Supp2.SupplierID AND B2.UnitPrice AND Supp2.CompanyName = 'supplier3');</p>
		</div>
		<button class="collapsible">3. Show the unique names of all books ordered by *lastname1 firstname1*.
</button>
		<div class="content">
		  <p>SELECT Distinct B.Title
	FROM db_book B, db_order O, db_order_detail OD, db_customer C
	WHERE B.BookID = OD.BookID AND OD.OrderID = O.OrderID AND O.CustomerID = C.CustomerID AND 
	C.LastName = 'lastname1' AND C.FirstName = 'firstname1';
</p>
		</div>
		<button class="collapsible">4. Show the title of books which have more than 10 units in stock.
</button>
		<div class="content">
		  <p>SELECT B.Title FROM db_book B WHERE B.Quantity > 10;</p>
		</div>

		<button class="collapsible">5. Show the total price *lastname1 firstname1* has paid for the books.
</button>
		<div class="content">
		  <p>SELECT SUM(OD.Quantity*B.UnitPrice) as Sum
	FROM db_book B, db_order_detail OD, db_order O, db_customer C
	WHERE OD.BookID = B.BookID AND O.OrderID = OD.OrderID AND C.CustomerID = O.CustomerID 
	AND C.FirstName = "firstname1" AND C.LastName="lastname1";
</p>
		</div>
		<button class="collapsible">6. Show the names of the customers who have paid less than $80 in totals.</button>
		<div class="content">
		  <p>SELECT FirstName, LastName from (SELECT C.FirstName as FirstName, C.LastName as LastName, 
	SUM(OD.Quantity*B.UnitPrice) AS PurchaseTotal
	FROM db_book B, db_order_detail OD, db_order O, db_customer C
	WHERE OD.BookID = B.BookID AND O.OrderID = OD.OrderID AND C.CustomerID = O.CustomerID 
	GROUP BY C.CustomerID HAVING PurchaseTotal < 80) as temporary;
</p>
		</div>
		<button class="collapsible">7. Show the name of books supplied by *supplier2*.
</button>
		<div class="content">
		  <p>SELECT B.Title
	FROM db_book B, db_supplier Sup
	WHERE B.SupplierID = Sup.SupplierID AND 
	Sup.CompanyName = 'supplier2';
</p>
		</div>
		<button class="collapsible">Show the total price each customer paid and their names. List the
result in descending price.
</button>
		<div class="content">
		  <p>SELECT C.FirstName, C.LastName, SUM(OD.Quantity*B.UnitPrice) AS PurchaseTotal
	FROM db_book B, db_order_detail OD, db_order O, db_customer C
	WHERE OD.BookID = B.BookID AND O.OrderID = OD.OrderID AND C.CustomerID = O.CustomerID 
	GROUP BY C.CustomerID ORDER BY PurchaseTotal DESC;
</p>
		</div>
		<button class="collapsible">9. Show the names of all the books shipped on 08/04/2016 and their shippers' names.</button>
		<div class="content">
		  <p>SELECT B.Title, Shp.ShpperName as ShipperNames
	FROM db_book B, db_shipper Shp, db_order O, db_order_detail OD
	WHERE B.BookID = OD.BookID AND OD.OrderID = O.OrderID AND O.ShipperID = Shp.ShipperID 
	AND O.ShippedDate = "8/4/2016";
</p>
		</div>
		<button class="collapsible">10. Show the unique names of all the books *lastname1 firstname1* and *lastname4 firstname4* *both* ordered.</button>
		<div class="content">
		  <p>SELECT Distinct B.Title
	FROM db_book B, db_order O, db_order_detail OD, db_customer C
	WHERE B.BookID = OD.BookID AND OD.OrderID = O.OrderID AND O.CustomerID = C.CustomerID 
	AND C.FirstName='firstname1' AND C.LastName='lastname1' AND B.BookID in (SELECT B1.bookID
		FROM db_book B1, db_order O1, db_order_detail OD1, db_customer C1
	   	WHERE B1.BookID = OD1.BookID AND OD1.OrderID = O1.OrderID AND O1.CustomerID = C1.CustomerID 
	   	AND C1.FirstName='firstname4' AND C1.LastName='lastname4');
</p>
		</div>
		<button class="collapsible">11. Show the names of all the books *lastname6 firstname6* was responsible for.</button>
		<div class="content">
		  <p>SELECT B.Title
	FROM db_book B, db_employee E, db_order O, db_order_detail OD
	WHERE OD.BookID = B.BookID AND OD.OrderID = O.OrderID AND O.EmployeeID = E.EmployeeID 
	AND E.FirstName = 'firstname6' AND LastName='lastname6';</p>
		</div>
		<button class="collapsible">12. Show the names of all the ordered books and their total quantities. List the result in ascending quantity.</button>
		<div class="content">
		  <p>SELECT B.Title, OD.Quantity
	FROM db_book B, db_order_detail OD
	WHERE B.BookID = OD.BookID ORDER BY OD.Quantity ASC;</p>
		</div>
		<button class="collapsible">13. Show the names of the customers who ordered at least 2 books.</button>
		<div class="content">
		  <p>SELECT FirstName, LastName from (select C.FirstName as FirstName, C.LastName as LastName, SUM(OD.Quantity) 
	AS TotalBooksOrdered FROM db_customer C, db_order O, db_order_detail OD
	WHERE OD.OrderID = O.orderID AND O.CustomerID = C.CustomerID 
	GROUP BY C.CustomerID HAVING TotalBooksOrdered >= 2) as Temp;
</p>
		</div>
		<button class="collapsible">14. Show the name of the customers who have ordered at least a book in *category3* or *category4* and the book names.</button>
		<div class="content">
		  <p>SELECT C.FirstName, C.LastName, B.Title
	FROM db_customer C, db_subject Sub, db_book B, db_order O, db_order_detail OD
	WHERE O.OrderID = OD.OrderID AND B.BookID = OD.BookID AND O.CustomerID = C.CustomerID 
	AND B.SubjectID = Sub.SubjectID AND (Sub.CategoryName = 'category3' OR Sub.CategoryName = 'category4');
	
</p>
		</div>
		<button class="collapsible">15. Show the name of the customer who has ordered at least one book written by *author1*.
</button>
		<div class="content">
		  <p>SELECT C.FirstName, C.LastName
	FROM db_order O, db_order_detail OD, db_customer C, db_book B
	WHERE O.OrderID = OD.OrderID AND O.CustomerID = C.CustomerID AND OD.BookID = B.BookID AND B.Author = 'author1';</p>
		</div>
		<button class="collapsible">16. Show the name and total sale (price of orders) of each employee.
</button>
		<div class="content">
		  <p>SELECT E.FirstName, E.LastName, TRUNCATE(SUM(B.UnitPrice * OD.Quantity),3) AS PriceOfOrder
	FROM db_book B, db_employee E, db_order O, db_order_detail OD
	WHERE O.OrderID = OD.OrderID AND B.BookID = OD.BookID AND O.EmployeeID = E.EmployeeID GROUP BY E.EmployeeID;
	
</p>
		</div>
		<button class="collapsible">17. Show the book names and their respective quantities for open orders (the orders which have not been shipped) at midnight 08/04/2016.</button>
		<div class="content">
		  <p>SELECT B.Title, sum(OD.Quantity) as Total
	FROM db_book B, db_order O, db_order_detail OD
	WHERE B.BookID = OD.BookID AND O.OrderID = OD.OrderID AND (O.ShippedDate is null or O.ShippedDate > '8/4/2016') 
	GROUP BY B.Title;</p>
		</div>
		<button class="collapsible">18. Show the names of customers who have ordered more than 1 book and the corresponding quantities.List the result in the descending quantity.</button>
		<div class="content">
		  <p>SELECT C.FirstName, C.LastName, SUM(OD.Quantity) AS Quant
	FROM db_customer C, db_order O, db_order_detail OD
	WHERE OD.OrderID = O.orderID AND O.CustomerID = C.CustomerID 
	GROUP BY C.CustomerID HAVING Quant > 1 ORDER BY Quant DESC;</p>
		</div>
		<button class="collapsible">19. Show the names of customers who have ordered more than 3 books and their respective telephone numbers.
</button>
		<div class="content">
		  <p>SELECT C.FirstName, C.LastName, C.Phone, SUM(OD.Quantity) AS Quant
	FROM db_customer C, db_order O, db_order_detail OD
	WHERE OD.OrderID = O.orderID AND O.CustomerID = C.CustomerID GROUP BY C.CustomerID HAVING Quant >3;</p>
		</div>

		<script>
		var coll = document.getElementsByClassName("collapsible");
		var i;

		for (i = 0; i < coll.length; i++) {
		  coll[i].addEventListener("click", function() {
		    this.classList.toggle("active");
		    var content = this.nextElementSibling;
		    if (content.style.maxHeight){
		      content.style.maxHeight = null;
		    } else {
		      content.style.maxHeight = content.scrollHeight + "px";
		    } 
		  });
		}
		</script>

	</body>
</html>

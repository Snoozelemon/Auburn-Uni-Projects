function eraseText() {
		    document.getElementById("input").value = "";
		    targetDiv.style.display = "none";
		}
		document.getElementById('db_book').onclick = function() {
            document.getElementById('input').innerHTML = 'SELECT * FROM db_book;';
        }

        document.getElementById('db_customer').onclick = function() {
            document.getElementById('input').innerHTML = 'SELECT * FROM db_customer;';
        }


        document.getElementById('db_employee').onclick = function() {
            document.getElementById('input').innerHTML = 'SELECT * FROM db_employee;';
        }

        document.getElementById('db_order').onclick = function() {
            document.getElementById('input').innerHTML = 'SELECT * FROM db_order;';
        }

        document.getElementById('db_order_detail').onclick = function() {
            document.getElementById('input').innerHTML = 'SELECT * FROM db_order_detail;';
        }

        document.getElementById('db_shipper').onclick = function() {
            document.getElementById('input').innerHTML = 'SELECT * FROM db_shipper;';
        }

        document.getElementById('db_subject').onclick = function() {
            document.getElementById('input').innerHTML = 'SELECT * FROM db_subject;';
        }

        document.getElementById('db_supplier').onclick = function() {
            document.getElementById('input').innerHTML = 'SELECT * FROM db_supplier;';
        }
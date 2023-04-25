<!Doctype html>
<html>
    <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="./img/website-icon.png" type="image/png">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>

        <style type="text/css">
            html{
                background-image: url('img/website-bg.jpg');
                background-repeat: no-repeat;
                background-size: cover;
                
            }
            h1{
                font-size: 50px;
            }
            hr{
                border: 2px solid whitesmoke;
                border-radius: 10px;
                width:70%;
                margin:0 auto;
            }
            #icon{
                padding-top: 10px;
                width:30px;
                height: 30px;
            }
            button{
                margin: 10px;
                padding: 15px;
                border: 2px solid whitesmoke;
                border-radius: 10px;
                text-align: center;
                font-size: 130%;
                background-color: Transparent;
                color: whitesmoke;
                display: inline-block;
                cursor: pointer;

            }
            .button1:hover {
                background-color: #4CAF50;
                color: white;
            }
            #main_div{
                
                margin: 0 auto;
                margin-top: 250px;
                padding: 20px;
                width: 800px;
                height: auto;
                text-align: center;  
                border: 2px solid white;
                border-radius: 10px;  
                color: whitesmoke;
                
                font-family: 'Archivo Black', sans-serif;
                font-family: 'EB Garamond', serif;
                font-family: 'Nunito', sans-serif;
            }
        .text{
            font-size: 150%;
        }
        </style>
      
        <title>Online Bookstore</title>
        
    </head>
    <body>
        <div id="main_div">
            <h1>Welcome to Online Bookstore</h1>
            <hr>
            <h2><strong>Created by Satya Sai Teja Charla <img src="./img/au.png" id="icon"></strong></h2>
            <h4>
                <i>This is my term project I've done for COMP-6120 class during my time at Auburn University (AU). In this project, I've created a database for a hypothetical online bookstore system.The system maintains information about books, subjects, supplier, and shippers. The bookstore acquires the desired books from suppliers, and ships the books by shippers. The system also keeps track of orders, customers, and employees.</i>
            </h4>
            <br>
            <button class="button1" onclick="window.location.href='https://webhome.auburn.edu/~szc0239/query_page.php';">Try Website <img src="./img/redirect.png"> </button>
            <button class="button1" onclick="window.location.href='https://webhome.auburn.edu/~szc0239/list_of_queries.php';">SQL Querier <img src="./img/redirect.png"></button>
        </div>
        
    </body>
</html>
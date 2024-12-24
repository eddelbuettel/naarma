
## Use duckdb with the adbc driver
library(adbcdrivermanager)
library(duckdb)
db <- adbc_database_init(duckdb_adbc())

con <- adbc_connection_init(db)

# Write a table
#swrite_adbc(nycflights13::flights, con, "flights")
write_adbc(data.frame(ind=1:2200, val=log(1:2200)), con, "flights")

## Query duckdb via adbc, get arrow streams, convert those
nadata <- read_adbc(con, "SELECT * from flights")  # return a array stream object
D <- nanoarrow::convert_array_stream(nadata)  	# now D is a data.frame
print(str(D))

library(naarma)
nadata <- read_adbc(con, "SELECT * FROM flights")  # return a array stream object
ns <- collectFromStream(nadata)
str(ns$get_next())
str(ns$get_next())

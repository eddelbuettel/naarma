
## Use duckdb with the adbc driver
library(adbcdrivermanager)
library(duckdb)
db <- adbc_database_init(duckdb_adbc())

con <- adbc_connection_init(db)

# Write a table
write_adbc(nycflights13::flights, con, "flights")
#write_adbc(data.frame(ind=seq(1.0,22.0,by=1.0), val=seq(22,1,by=-1)), con, "flights")

## Query duckdb via adbc, get arrow streams, convert those
#nadata <- read_adbc(con, "SELECT * from flights")  # return a array stream object
#D <- nanoarrow::convert_array_stream(nadata)  	# now D is a data.frame
#print(str(D))

library(naarma)
nadata <- read_adbc(con, "SELECT * FROM flights")  # return a array stream object

#print(nadata)
#print(nadata$get_schema())
#print(nadata$get_next())
ns <- collectFromStream(nadata, verbose=FALSE)
print(str(ns))


## Use duckdb with the adbc driver
suppressMessages({
    library(adbcdrivermanager)
    library(duckdb)
    library(naarma)
    #library(bit64)  # to test with as.integer64
})

db <- adbc_database_init(duckdb_adbc())

con <- adbc_connection_init(db)

# Write a table
#write_adbc(nycflights13::flights, con, "flights")
#write_adbc(data.frame(ind = as.integer64(seq(1.0,2200.0,by=1.0)),
#                      val = as.integer64(seq(2200,1,by=-1))),
#           con, "flights")
write_adbc(data.frame(ind = 1:2200, val=seq(2200,1,by=-1)), con, "flights")

## Query duckdb via adbc, get arrow streams, convert those
#nadata <- read_adbc(con, "SELECT * from flights")  # return a array stream object
#D <- nanoarrow::convert_array_stream(nadata)  	# now D is a data.frame
#print(str(D))

nadata <- read_adbc(con, "SELECT * FROM flights")  # return a array stream object

#print(nadata)
#print(nadata$get_schema())
#print(nadata$get_next())
ns <- collectFromStream(nadata, verbose=FALSE)
print(str(ns))

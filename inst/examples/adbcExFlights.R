
con <- adbc_connection_init(db)

# Write a table
write_adbc(nycflights13::flights, con, "flights")

## Query duckdb via adbc, get arrow streams, convert those
nadata <- read_adbc(con, "SELECT * from flights")  # return a array stream object
D <- nanoarrow::convert_array_stream(nadata)  	# now D is a data.frame

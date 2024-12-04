library(stringr)

X <- unlist(read.delim("input.txt", header=FALSE))
X <- str_split_fixed(X, "", nchar(X[1]))

vflip <- function(M) {
  M[, ncol(M):1]
}


get_nsub <- function(x, n = 4) {
  X <- matrix(NA, nrow = length(x) - n + 1, ncol = n)
  for (i in seq_len(length(x) - n + 1)) {
    X[i, ] <- x[i:(i + n - 1)]
  }
  X
}

get_substrings <- function(x, n = 4) {
  if (length(x) < n) return(NULL)
  X <- get_nsub(as.character(x), n = n)

  apply(X, MARGIN = 1, FUN = stringr::str_c, collapse = "")
}


get_diag_from_point <- function(M, i, j) {
  min <- min(ncol(M), nrow(M))
  diag(M[i:min, j:min, drop=FALSE])
}


MATCHES <- c("XMAS", "SAMX")

# rows
sum = 0
for (i in seq_len(nrow(X))) {
  row <- X[i, ]
  sum <- sum + sum(get_substrings(row) %in% MATCHES)
}

# cols
for (i in seq_len(ncol(X))) {
  col <- X[, i]
  sum <- sum + sum(get_substrings(col) %in% MATCHES)
}

# diag (non-flipped) 
for (j in seq_len(ncol(X))) {
  x <- get_diag_from_point(X, i=1, j=j)
  sum <- sum + sum(get_substrings(x) %in% MATCHES)
}

for (i in seq_len(nrow(X))[-1]) {
  x <- get_diag_from_point(X, i=i, j=1)
  sum <- sum + sum(get_substrings(x) %in% MATCHES)
}

# diag (flipped) 
Y <- vflip(X)
for (j in seq_len(ncol(Y))) {
  x <- get_diag_from_point(Y, i=1, j=j)
  sum <- sum + sum(get_substrings(x) %in% MATCHES)
}

for (i in seq_len(nrow(Y))[-1]) {
  x <- get_diag_from_point(Y, i=i, j=1)
  sum <- sum + sum(get_substrings(x) %in% MATCHES)
}

print(sum)

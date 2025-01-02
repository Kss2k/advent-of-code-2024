niter <- 100
path  <- "input.txt"
nrow  <- 103
ncol  <- 101


readfile <- function(path) {
  read.delim(path, header=FALSE, sep="\t") |>
    unlist() |> 
    stringr::str_split_fixed("p=| v=|,", 5) |>
    as.data.frame() |>
    dplyr::select(2:5) |> 
    apply(MARGIN=2, as.numeric) |>
    as.data.frame() |>
    dplyr::rename(p.x=V2, p.y=V3, v.x=V4, v.y=V5)
}


calibrate <- function(x, n) {
  k <- n # - 1 # p.x/p.y are zero-based
  y <- ifelse(x < 0, yes = n - (-x %% k), no = x %% k)

  ifelse(y == n, yes=0, no=y)
}


calculate <- function(input, n, nrow, ncol) {
  p.x <- input$p.x + n * input$v.x
  p.y <- input$p.y + n * input$v.y
  
  input$p.x <- calibrate(p.x, n=ncol)
  input$p.y <- calibrate(p.y, n=nrow)

  input
}


printf <- function(...) {
  cat(sprintf(...))
}


getmap <- function(input, nrow, ncol) {
  map <- matrix(0L, nrow=nrow, ncol=ncol)
  for (i in seq_len(nrow)) for (j in seq_len(ncol)) {
    match <- which(input$p.x == j - 1 & input$p.y == i - 1)
    map[i, j] <- length(match)
  }

  map
}


printmap <- function(input, nrow, ncol) {
  map <- getmap(input, nrow=nrow, ncol=ncol)

  for (i in seq_len(nrow)) {
    for (j in seq_len(ncol)) {
      if (map[i, j]) 
        cat(map[i, j])
      else 
        cat(".")
    }
    cat("\n")
  }
}


printmap <- function(input, nrow, ncol) {
  for (i in seq_len(nrow) - 1) {
    for (j in seq_len(ncol) - 1) {
      match <- which(input$p.x == j & input$p.y == i)

      if (length(match)) 
        cat(length(match))
      else 
        cat(".")
    }

    cat("|\n")
  }
}


longestLine <- function(x) {
  y <- numeric(length(x) - 1)
  s <- 1
  for (i in seq_along(x)[-1]) {
    if (x[i] == x[i - 1] + 1) s <- s + 1
    else s <- 1

    y[i - 1] <- s
  }

  max(y)
}


longestVline <- function(result) {
  sorted <- result[order(result$p.x, result$p.y), ]
  longestLine(sorted$p.y)
}


quadrant <- function(map, i, j) {
  qi <- (nrow(map) + 1) / 2
  qj <- (ncol(map) + 1) / 2

  seq_i <- if (i < 2) 1:(qi - 1) else (qi + 1):nrow(map)
  seq_j <- if (j < 2) 1:(qj - 1) else (qj + 1):ncol(map)

  map[seq_i, seq_j]
}


safetyfactor <- function(input, nrow, ncol) {
  map    <- getmap(input, nrow=nrow, ncol=ncol)
  guards <- numeric(0L)

  for (i in seq_len(2)) {
    for (j in seq_len(2)) {
      gq <- sum(quadrant(map, i=i, j=j))
      guards <- c(guards, gq)
    }
  }

  prod(guards)
}


input <- readfile(path)
result <- calculate(input, n=niter, nrow=nrow, ncol=ncol)
printmap(result, nrow=nrow, ncol=ncol)
safetyfactor(result, nrow=nrow, ncol=ncol)


for (i in seq_len(100000) - 1) {
  result <- calculate(input, n=i, nrow=nrow, ncol=ncol)
  l      <- longestVline(result)

  printf("Seconds = %i, Longest Vline = %i\n", i, l)
  if (l >= 10) {
    printmap(result, nrow=nrow, ncol=ncol)
    break
  }
}

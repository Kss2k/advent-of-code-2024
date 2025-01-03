reindeer <- c("^", "v", ">", "<")

readfile <- function(filepath) {
  raw <- unlist(read.delim(filepath, header=FALSE, sep="\t"))
  out <- stringr::str_split_fixed(raw, pattern="", n=nchar(raw[1]))
  out[out == "S"] <- ">"
  out
}


get_visited <- function(input) {
  x <- input == '#'
  matrix(x, nrow=nrow(input), ncol=ncol(input))
}


get_distances <- function(input) {
  d <- matrix(Inf, nrow=nrow(input), ncol=ncol(input))
  d[input %in% reindeer] <- 0
  d
}


which_ij <- function(x, ncol, unique=TRUE) {
  k <- if (unique) which(x)[1] else which(x)
  i <- k %% ncol
  j <- (k - i) / ncol + 1

  c(i, j) 
}


find_reindeer <- function(input) {
  ncol <- ncol(input)
  which_ij(input %in% reindeer, ncol=ncol)
}


rotations <- function(reindeer, x, y) {
  ifelse(reindeer == x, yes=0,
         no=ifelse(reindeer == y, yes=2, no=1))
}


get_neighbours <- function(map, 
                           visited, i0, j0, 
                           rotation_distance = 1000) {
  reindeer <- map[i0, j0]
  header   <- c("i", "j", "distance", "rotations", "direction")

  directions <- matrix(c(
    i0 - 1, j0, NA, rotations(reindeer, "^", "v"), 1, # up
    i0 + 1, j0, NA, rotations(reindeer, "v", "^"), 2, # down
    i0, j0 + 1, NA, rotations(reindeer, ">", "<"), 3, # right
    i0, j0 - 1, NA, rotations(reindeer, "<", ">"), 4  # left
  ), nrow=4, ncol=5, dimnames = list(NULL, header), byrow=TRUE)

  for (i in seq_len(NROW(directions))) {
    id <- directions[i, 1]
    jd <- directions[i, 2]

    if (id < 1 || id > nrow(map) || jd < 1 || jd > ncol(map) ||
        visited[id, jd]) next

    directions[i, "distance"] <- rotation_distance * directions[i, "rotations"] + 1
  }

  directions[!is.na(directions[, "distance"]), , drop=FALSE]
}


str_col <- function(x, c = "") paste0(x, collapse=c)
printf <- function(...) cat(sprintf(...))


printm <- function(input, coords=FALSE) {
  if (coords) {
    header <- seq_len(ncol(input)) %% 10
    printf("      %s\n", str_col(header))
  }

  for (i in seq_len(nrow(input))) {
    if (coords) 
      printf("[,%2i] ", i)

    printf("%s\n", str_col(input[i, ]))
  }
}


inf_mask <- function(X, mask) {
  X[mask] <- Inf
  X
}


MAX_RECURSE <- 100
djikstra <- function(map, visited, distances, sdistances, depth=1) {
  uv_distances <- inf_mask(distances, visited)
  condition    <- uv_distances == min(uv_distances) & !visited

  ij <- which_ij(condition, ncol=ncol(map))

  if (all(is.infinite(distances)) || all(visited) || !length(ij) || depth >= MAX_RECURSE) {
    out <- list(map=map, visited=visited, distances=distances, 
                sdistances=sdistances, finished=depth < MAX_RECURSE)
    return(out)
  }

  i0 <- ij[1]
  j0 <- ij[2]


  visited[i0, j0] <- TRUE
  directions      <- get_neighbours(map=map, visited=visited, i=i0, j=j0)
 
  for (i in seq_len(NROW(directions))) {
    i1        <- directions[i, "i"]
    j1        <- directions[i, "j"]
    distance  <- directions[i, "distance"]
    direction <- directions[i, "direction"]


    if (distance + distances[i0, j0] < distances[i1, j1]) {
      map[i1, j1] <- reindeer[direction]
      distances[i1, j1] <- distance + distances[i0, j0]
      sdistances[i1, j1] <- sdistances[i0, j0] + 1
    }
  }

  djikstra(map=map, visited=visited, distances=distances, 
           sdistances=sdistances, depth=depth+1)
}


backtrace <- function(input, sdistances, i0, j0) {
  input[i0, j0] <- "P"
  if (input[i0, j0] == 'S')
    return(input)

  step_i <- c(1, -1, 0, 0)
  step_j <- c(0, 0, 1, -1)

  for (k in seq_along(step_i)) {
    i1 <- i0 + step_i[k]
    j1 <- j0 + step_j[k]

    if (i1 < 1 || i1 > nrow(input) || j1 < 1 || j1 > ncol(input) || 
        input[i1, j1] == '#' || sdistances[i1, j1] != sdistances[i0, j0] - 1) 
      next

    input <- backtrace(input, sdistances, i1, j1)
  }

  input
}


find_path <- function(filepath) {
  input      <- readfile(filepath)
  map        <- input
  visited    <- get_visited(input)
  distances  <- get_distances(input)
  sdistances <- distances
  finished   <- FALSE

  while (!finished) {
    result     <- djikstra(map, visited=visited, distances=distances, sdistances=sdistances)
    map        <- result$map
    visited    <- result$visited
    distances  <- result$distances
    sdistances <- result$sdistances
    finished   <- result$finished
  }

  input[input %in% reindeer] <- "S"
  ij <- which_ij(input == "E", ncol=ncol(input))
  seatings <- backtrace(input=input, sdistances=sdistances, 
                        i0=ij[1], j0=ij[2])
  list(shortest_path=result$distances[input == "E"],
       nseatings=sum(seatings == "P"))
}


filepath <- "input.txt"
result <- find_path(filepath)

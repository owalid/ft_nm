mkdir -p .out

./ft_nm $1 > .out/my_nm.out
nm $1 > .out/nm.out
diff --color .out/my_nm.out .out/nm.out

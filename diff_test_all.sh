./diff_simple.sh $1

./ft_nm $1 -r > .out/my_nm_r.out
nm $1 -r > .out/nm_r.out
diff --color .out/my_nm_r.out .out/nm_r.out


./ft_nm $1 -u > .out/my_nm_u.out
nm $1 -u > .out/nm_u.out
diff --color .out/my_nm_u.out .out/nm_u.out


./ft_nm $1 -p > .out/my_nm_p.out
nm $1 -p > .out/nm_p.out
diff --color .out/my_nm_p.out .out/nm_p.out

./ft_nm $1 -g > .out/my_nm_g.out
nm $1 -g > .out/nm_g.out
diff --color .out/my_nm_g.out .out/nm_g.out

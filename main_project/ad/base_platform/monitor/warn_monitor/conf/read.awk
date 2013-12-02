{
	if ($0 ~ /\[.*\]/)
	{
		split($0, arr, "[][]");
		service = arr[2];
	}
	else if ($0 ~ /^#.*/)
		; # comment
	else if (NF == 0)
		; # blank line
	else
	{
		threshold[service, $1, "email"] = $2;
		threshold[service, $1, "sms"] = $3;
	}
}

END {
	for (i in threshold)
		print i, threshold[i];
}

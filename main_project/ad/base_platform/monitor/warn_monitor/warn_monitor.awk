BEGIN {
	OFMT="%.2g";
	CONVFMT="%.2g";
	time = strftime("%T", systime());
}

ARGIND == 1 {
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

ARGIND > 1 && $7 > 0 {
	match($2, "^[a-zA-Z]*");
	service = substr($2, 0, RLENGTH);

	rate_num["warn"] = $3;
	rate_num["ice_timeout"] = $4;
	rate_num["connect_timeout"] = $5;

	for (i in rate_num)
	{
		rate = rate_num[i] / $7 * 100;
		if ((service, i, "email") in threshold && rate > threshold[service, i, "email"])
			print $1, $2, rate_num[i], $7, rate "%", time >> "warnmsg/" i "_email";
		if ((service, i, "sms") in threshold && rate > threshold[service, i, "sms"])
			print $1, $2, rate_num[i], $7, rate "%", time >> "warnmsg/" i "_sms";
	}

	num_num["fatal"] = $6;
	for (i in num_num)
	{
		if ((service, i, "email") in threshold && num_num[i] > threshold[service, i, "email"])
			print $1, $2, num_num[i], $7, time >> "warnmsg/" i "_email";
		if ((service, i, "sms") in threshold && num_num[i] > threshold[service, i, "sms"])
			print $1, $2, num_num[i], $7, time >> "warnmsg/" i "_sms";
	}
}

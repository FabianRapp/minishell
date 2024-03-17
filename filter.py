
source_file = "failed_tests_details.txt"
target_file = "filtered_failed_tests.txt"

filter_strs = ["-n"]

def	save_block(source):
	with open(target_file, "a") as target:
		line = source.readline()
		while line:
			target.write(line)
			if (line.strip() == "------"):
				return
			line = source.readline()

def	insert_count(count):
	with open (target_file, 'r') as file:
		content = file.readlines()
	with open (target_file, 'w') as file:
		file.write(f"Total errors after filtering: {count}\n------\n")
		file.writelines(content)


if	__name__ == '__main__':
	try:
		tmp = open(target_file, "w")
		tmp.close()
	except:
		pass
	count = 0
	with open(source_file, 'r') as file:
		found_filter = False
		last_block = 0
		line = file.readline()
		while line:
			if last_block == 0:
				last_block = file.tell() - len(line)
			for filter in filter_strs:
				if filter in line:
					found_filter = True
					break
			if line.strip() == "------":
				if not found_filter:
					file.seek(last_block)
					save_block(file)
					count += 1
				found_filter = False
				last_block = 0
			line = file.readline()
	insert_count(count)

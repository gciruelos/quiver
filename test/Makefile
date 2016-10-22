END_TO_END_DIR = end-to-end/ 
END_TO_END_TESTS = $(wildcard end-to-end/*.expected)

.PHONY: all end-to-end

all: end-to-end

end-to-end: $(END_TO_END_TESTS)
	for t in end-to-end/*.expected ; do \
		eval $$(python end-to-end.py $$t) | diff -s $$t - ; \
	done



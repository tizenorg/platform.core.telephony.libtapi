SUBDIRS = common/unit call/unit gprs/unit network/unit power/unit sat/unit sim/unit sms/unit sound/unit ss/unit

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean :
	-for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean ); done


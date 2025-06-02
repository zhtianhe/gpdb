
#include "postgres.h"

#include "utils/builtins.h"

#include "cdb/cdbutil.h"

PG_MODULE_MAGIC;
void _PG_init(void);

PG_FUNCTION_INFO_V1(pg_event_trigger_ddl_commands);
PG_FUNCTION_INFO_V1(pg_event_trigger_table_rewrite_oid);
PG_FUNCTION_INFO_V1(pg_event_trigger_table_rewrite_reason);

Datum
pg_event_trigger_ddl_commands(PG_FUNCTION_ARGS)
{
    return pg_event_trigger_ddl_commands_internal(fcinfo);
}

Datum
pg_event_trigger_table_rewrite_oid(PG_FUNCTION_ARGS)
{
    return pg_event_trigger_table_rewrite_oid_internal(fcinfo);
}

Datum
pg_event_trigger_table_rewrite_reason(PG_FUNCTION_ARGS)
{
    return pg_event_trigger_table_rewrite_reason_internal(fcinfo);
}

PG_FUNCTION_INFO_V1(gp_add_segment_aux_mirror);

Datum
gp_add_segment_aux_mirror(PG_FUNCTION_ARGS)
{
	GpSegConfigEntry new;

	if (PG_ARGISNULL(0))
		elog(ERROR, "contentid cannot be NULL");
	new.segindex = PG_GETARG_INT16(0);

	if (PG_ARGISNULL(1))
		elog(ERROR, "hostname cannot be NULL");
	new.hostname = TextDatumGetCString(PG_GETARG_DATUM(1));

	if (PG_ARGISNULL(2))
		elog(ERROR, "address cannot be NULL");
	new.address = TextDatumGetCString(PG_GETARG_DATUM(2));

	if (PG_ARGISNULL(3))
		elog(ERROR, "port cannot be NULL");
	new.port = PG_GETARG_INT32(3);

	if (PG_ARGISNULL(4))
		elog(ERROR, "datadir cannot be NULL");
	new.datadir = TextDatumGetCString(PG_GETARG_DATUM(4));

/* XXX: todo - check
	mirroring_sanity_check(MASTER_ONLY | SUPERUSER, "gp_add_segment_aux_mirror");
*/


	new.dbid = get_availableDbId();
	new.mode = GP_SEGMENT_CONFIGURATION_MODE_NOTINSYNC;
	new.status = GP_SEGMENT_CONFIGURATION_STATUS_DOWN;
	new.role = GP_SEGMENT_CONFIGURATION_ROLE_AUX_MIRROR;
	new.preferred_role = GP_SEGMENT_CONFIGURATION_ROLE_AUX_MIRROR;

	add_segment(&new);

	PG_RETURN_INT16(new.dbid);
}

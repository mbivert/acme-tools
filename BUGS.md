``bin/Autos`` and the hooks mechanism (``bin/Hook``,
``bin/Unhook``) both crashes acme reproducibly enough
here.

I haven't investigated further; for the record, it crashed
upon executing ``Unhook`` in a ``win(1)`` buffer, and some
crashes seems to be related to the autodump mechanism
in ``bin/Acme`` (which calls ``XDump``, which itself calls
``Hook/Unhook``).


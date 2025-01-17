[Unsupported] Developing inside Docker with precompiled TPL binaries
====================================================================

For development purposes, you may want to use the publicly available docker images or the OSX tarball instead of compiling them yourself.
While this is possible and this page will help you in through this journey, please note that *this is not officially supported by the GEOSX team that reserves the right to modify its workflow or delete elements on which you may have build your own workflow*.

There are multiple options to use the exposed docker images.

- A lot of IDE now provide remote development modes (e.g. `CLion <https://www.jetbrains.com/help/clion/remote-projects-support.html>`_, `VS Code <https://code.visualstudio.com/docs/remote/remote-overview>`_, `Eclipse Che <https://www.eclipse.org/che/>`_ and surely others).
  Depending on your choice, please read their documentation carefully so you can add their own requirements on top the TPL images that are already available.
- Another option is to develop directly inside the container (*i.e.* not remotely).
  Install your favorite development inside the image (be mindful of X display issues), connect to the running container and start hacking!

You must first `install docker <https://docs.docker.com/get-docker/>`_ on your machine.
Note that there now exists a `rootless install <https://docs.docker.com/engine/security/rootless/>`_ that may help you in case you are not granted extended permissions on your environment.
Also be aware that nvidia provides its own `nvidia-docker <https://github.com/NVIDIA/nvidia-docker>`_ that grants access to GPUs.

Once you've installed docker, you must select from our `docker registry <https://hub.docker.com/u/geosx/>`_ the target environment you want to develop into.

- You can select the distribution you are comfortable with, or you may want to mimic (to some extend) a production environment.
- Our containers are built with a relative CPU agnosticism (still ``x86_64``), so you should be fine.
- Our GPU containers are built for a dedicated ``compute capability`` that may not match yours. Please dive into our configuration files and refer to the `official nvidia page <https://developer.nvidia.com/cuda-gpus>`_ to see what matches your needs.
- There may be risks of kernel inconsistency between the container and the host, but if you have relatively modern systems (and/or if you do not interact directly with the kernel like ``perf``) it should be fine.
- You may have noticed that our docker containers are tagged like ``155-669``. Please refer to :ref:`_Continuous_Integration_process` for further information.

Now that you've selected your target environment, you must be aware that just running a TPL docker image is not enough to let you develop.
You'll have to add extra tools.

The following `example` is for our ``ubuntu`` flavors.
You'll notice the arguments ``IMG``, ``VERSION``, ``ORG``.
While surely overkill for most cases, if you develop in GEOSX on a regular basis you'll appreciate being able to switch containers easily.
For example, simply create the image ``remote-dev-ubuntu18.04-gcc8:156-642`` by running

.. code-block:: console

    export VERSION=156-642
    export IMG=ubuntu18.04-gcc8
    export REMOTE_DEV_IMG=remote-dev-${IMG}
    docker build --build-arg ORG=geosx --build-arg IMG=${IMG} --build-arg VERSION=${VERSION} -t ${REMOTE_DEV_IMG}:${VERSION} -f /path/to/Dockerfile .

And the ``Dockerfile`` is the following (comments are embedded)

.. code-block:: console

    # Define you base image for build arguments
    ARG IMG
    ARG VERSION
    ARG ORG
    FROM ${ORG}/${IMG}:${VERSION}

    # Uninstall some packages, install others.
    # I use those for clion, but VS code would have different requirements.
    # Use yum's equivalent commands for centos/red-hat images.
    # Feel free to adapt.
    RUN apt-get update
    RUN apt-get remove --purge -y texlive graphviz
    RUN apt-get install --no-install-recommends -y openssh-server gdb rsync gdbserver ninja-build ca-certificates

    # You will need cmake to build GEOSX.
    ARG CMAKE_VERSION=3.16.8
    RUN apt-get install -y --no-install-recommends curl ca-certificates && \
        curl -fsSL https://cmake.org/files/v${CMAKE_VERSION%.[0-9]*}/cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz | tar --directory=/usr/local --strip-components=1 -xzf - && \
        apt-get purge --auto-remove -y curl ca-certificates
    RUN apt-get autoremove -y

    # You'll most likely need ssh/sshd too (e.g. CLion and VSCode allow remote dev through ssh).
    # This is the part where I configure sshd.

    # I'm developing in a version of docker that requires root.
    # So by default I use root. Feel free to adapt.
    RUN echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config
    RUN echo "PermitUserEnvironment yes" >> /etc/ssh/sshd_config
    RUN mkdir -p -m 700 /root/.ssh
    # Put your own public key here!
    RUN echo "ssh-rsa AAAAB3NzaC1yc2 ... 0zYY+1eKEinP5Q== somebody@somewhere.com" > /root/.ssh/authorized_keys

    # Some important variables are provided through the environment.
    # You need to explicitly tell sshd to forward them.
    # Using these variables and not paths will let you adapt to different installation locations in different containers.
    # Feel free to adapt to your own convenience.
    RUN touch /root/.ssh/environment &&/
        echo "CC=${CC}" >> /root/.ssh/environment &&\
        echo "CXX=${CXX}" >> /root/.ssh/environment &&\
        echo "MPICC=${MPICC}" >> /root/.ssh/environment &&\
        echo "MPICXX=${MPICXX}" >> /root/.ssh/environment &&\
        echo "MPIEXEC=${MPIEXEC}" >> /root/.ssh/environment &&\
        echo "OMPI_CC=${CC}" >> /root/.ssh/environment &&\
        echo "OMPI_CXX=${CXX}" >> /root/.ssh/environment &&\
        echo "GEOSX_TPL_DIR=${GEOSX_TPL_DIR}" >> /root/.ssh/environment

    # This is the default ssh port that we do not need to modify.
    EXPOSE 22
    # sshd's option -D prevents it from detaching and becoming a daemon.
    # Otherwise, sshd would not block the process and `docker run` would quit.
    RUN mkdir -p /run/sshd
    ENTRYPOINT ["/usr/sbin/sshd", "-D"]

Now that you've created the image, you must instantiate it as a container.
I like to do

.. code-block:: console

    docker run --cap-add=ALL -d --name ${REMOTE_DEV_IMG}-${VERSION} -p 64000:22 ${REMOTE_DEV_IMG}:${VERSION}

that creates the container ``remote-dev-ubuntu18.04-gcc8-156-642``, running instance of ``remote-dev-ubuntu18.04-gcc8:156-642``.

- Note that you'll have to access your remote development instance though port ``64000`` (forwarded to standard port ``22`` by docker).
- Please be aware of how to retrieve your code back: you may want to bind mount volumes and store you code there (``-v``/``--volume=`` options of `docker run <https://docs.docker.com/engine/reference/run/>`_).
- Change ``docker`` to ``nvidia-docker`` and add the ``--gpus=...`` option for GPUs.

You can stop and restart your container with

.. code-block:: console

    docker stop ${REMOTE_DEV_IMG}-${VERSION}
    docker start ${REMOTE_DEV_IMG}-${VERSION}

Now hack.

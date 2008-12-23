(load "template")

(set template
<<-END
<h1>Hello World</h1>
<pre>
<%= (request unparsed_uri) %>
<%= (request uri) %>
<%= (request filename) %>
<%= (request path_info) %>
<%= (request args) %>
<% ((request headers) each:(do (key value) %><%= key %>: <%= value %>
<% )) %>
</pre>
<p>
This message is coming to you from a Nu script being run by an Apache server using the 
<a href="http://github.com/timburks/mod_nu/tree/master">mod_nu</a> extension.
</p>
<p>
The Apache server is running on Debian Linux in VMware Fusion on Mac OS X Server.
</p>
<p>
Nu now runs on devices ranging from iPhones to virtualized cloud servers.
<p>
From the pocket to the cloud, <a href="http://programming.nu">Nu</a>.
</p>

<h3>NSString instance methods</h3>
<ul>
<% ((NSString instanceMethods) each:(do (m) %>
<li>
<%= (m name) %>
</li>
<% )) %>
</ul>
END)

;; (eval (NuTemplate codeForString:template))

(form description)
